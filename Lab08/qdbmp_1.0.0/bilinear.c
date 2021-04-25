unsigned int getR(unsigned int in)
{
	return ((in & 0x00ff0000) >> 16);
}
unsigned int getG(unsigned int in)
{
	return ((in & 0x0000ff00) >> 8);
}
unsigned int getB(unsigned int in)
{
	return ((in & 0x000000ff));
}
void CImageProcessor::Rotate(Bitmap * pBitmap, double angle)
{
	// This function effectively rotates an image, with bilinear filtering

	int width = pBitmap->GetWidth();
	int height = pBitmap->GetHeight();
	double cX = (double)width/2.0f;
	double cY = (double)height/2.0f;
	// This code uses GDI+. For performance reasons, the bitmap is locked so we can work with the memory directly

	BitmapData bitmapData;
	pBitmap->LockBits(&Rect(0,0,pBitmap->GetWidth(), pBitmap->GetHeight()), ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData);
	unsigned int *pRawBitmapOrig = (unsigned int*)bitmapData.Scan0;   // for easy access and indexing

	unsigned int *pBitmapCopy = new unsigned int[bitmapData.Stride*height/4];
	memcpy(pBitmapCopy, pRawBitmapOrig, (bitmapData.Stride*height/4) * sizeof(unsigned int));
	int nPixels = height*bitmapData.Stride/4;
	#pragma omp parallel for
	for (int i=0; i < height; i++)
	{
		double relY = cY-i;		// get center y coordinate
		for (int j=0; j < width; j++)
		{
			double relX = j-cX;	// get center x coordinate
			// do rotation transformation
			double xPrime = relX*cos(angle) + relY*sin(angle);
			double yPrime = -1 * relX*sin(angle) + relY*cos(angle);
			// re-center pixel
			xPrime += cX;
			yPrime += cY;
			// There are four nearest original pixels, q11, q12, q21, and q22
			// While we may get away with using only four variables, this code
			// seperates out the x and y of each point for clarity reasons.
			// Most compilers should be capable of optimizing away the redundant
			// steps here.

			int q12x = (int)floor(xPrime);
			int q12y = (int)floor(yPrime);
			q12x = max(0, q12x);
			q12y = max(0, q12y);
			q12x = min(width-1, q12x);
			q12y = min(height-1, q12y);
			int q22x = (int)ceil(xPrime);
			int q22y = q12y;
			q22x = min(width-1, q22x);
			q22x = max(0, q22x);
			int q11x = q12x;
			int q11y = (int)ceil(yPrime);
			q11y = min(height-1, q11y);
			q11y = max(0, q11y);
			int q21x = q22x;
			int q21y = q11y;
			// We need to get the four nearest neighbooring pixels.
			// Pixels which are past the border of the image are clamped to the border already.
			unsigned int q11 = pBitmapCopy[q11y*bitmapData.Stride/4 + q11x];
			unsigned int q12 = pBitmapCopy[q12y*bitmapData.Stride/4 + q12x];
			unsigned int q21 = pBitmapCopy[q21y*bitmapData.Stride/4 + q21x];
			unsigned int q22 = pBitmapCopy[q22y*bitmapData.Stride/4 + q22x];
			
			// Here, it would be better to use a vector class to store the R,G and B values
			// to keep code consise. But they have been seperated out for maximum clarity
			// and simplicity.

			unsigned int q11r = getR(q11);
			unsigned int q11g = getG(q11);
			unsigned int q11b = getB(q11);
			unsigned int q12r = getR(q12);
			unsigned int q12g = getG(q12);
			unsigned int q12b = getB(q12);
			unsigned int q21r = getR(q21);
			unsigned int q21g = getG(q21);
			unsigned int q21b = getB(q21);
			unsigned int q22r = getR(q22);
			unsigned int q22g = getG(q22);
			unsigned int q22b = getB(q22);
			double factor1;
			double factor2;
			
			if ( q21x == q11x ) // special case to avoid divide by zero
			{
				factor1 = 1; // They're at the same X coordinate, so just force the calculatione to one point
				factor2 = 0;
			}
			else
			{
				factor1 = (((double)q21x - (double)xPrime)/((double)q21x - (double)q11x));
				factor2 = (((double)xPrime - (double)q11x)/((double)q21x - (double)q11x));
			}
			double R1r = factor1 * (double)q11r + factor2*(double)q21r;
			double R1g = factor1 * (double)q11g + factor2*(double)q21g;
			double R1b = factor1 * (double)q11b + factor2*(double)q21b;
			double R2r = factor1 * (double)q12r + factor2*(double)q22r;
			double R2g = factor1 * (double)q12g + factor2*(double)q22g;
			double R2b = factor1 * (double)q12b + factor2*(double)q22b;
			double factor3;
			double factor4;
			if (q12y == q11y) // special case to avoid divide by zero
			{
				factor3 = 1;
				factor4 = 0;
			}
			else
			{
				factor3 = ((double) q12y - yPrime)/((double)q12y - (double)q11y);
				factor4 = (yPrime - (double)q11y)/((double)q12y - (double)q11y);
			}
			// Calculate the final unbounded RGB values
			unsigned int finalR = (unsigned int)((factor3 * R1r) + (factor4*R2r));
			unsigned int finalG = (unsigned int)((factor3 * R1g) + (factor4*R2g));
			unsigned int finalB = (unsigned int)((factor3 * R1b) + (factor4*R2b));
			// Clamp the RBG values to a value between 0 and 255
			finalR = min(255, finalR);
			finalR = max(0, finalR);
			finalG = min(255, finalG);
			finalG = max(0, finalG);
			finalB = min(255, finalB);
			finalB = max(0, finalB);
			unsigned int finalPixel = 0xff000000 | (finalR << 16) | (finalG << 8) | finalB;
			pRawBitmapOrig[(height-i-1)*bitmapData.Stride/4 + j] = finalPixel;
			//pRawBitmapOrig[(height-i-1)*bitmapData.Stride/4 + j] = q11; // This produces non-interpolated image for comparison
			// Note that we actually flip the image here to put it back into screen coordinates.
		}
	}
	delete[] pBitmapCopy;
	pBitmap->UnlockBits(&bitmapData);
}