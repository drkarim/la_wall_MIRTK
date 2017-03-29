#include "mirtk/Common.h"
#include "mirtk/Options.h"
#include "mirtk/IOConfig.h"
#include "mirtk/GenericImage.h"

using namespace mirtk;

Array<double> i_ctr; 

void GetSlice(GreyImage* img1, GreyImage& img_crop, int in_x, int in_y, int in_z)
{
	int maxX, maxY, maxZ;
	maxX = img1->GetX(); 
	maxY = img1->GetY(); 
	maxZ = img1->GetZ(); 
	int n=0;

	if (in_x > 0 || in_y > 0 || in_z > 0) 
	{
		if (in_x > 0)
		{
			img_crop = img1->GetRegion(in_x, 0,0, in_x+1, maxY, maxZ); 
		}
		else if (in_y > 0)
		{
			img_crop = img1->GetRegion(0, in_y,0, maxX, in_y+1, maxZ); 
		}
		else if (in_z > 0)
		{
			img_crop = img1->GetRegion(0, 0,in_z, maxX, maxY, in_z+1);
		}
	}
}

void GetSliceDice(GreyImage* img1_slice, GreyImage* img2_slice, double& sorensen_dice)
{
	int maxX, maxY, maxZ; 
	double tot_img1=0, tot_img2=0, tot_img1_and_img2=0; 
	
	maxX = img1_slice->GetX(); 
	maxY = img1_slice->GetY(); 
	maxZ = img1_slice->GetZ(); 

	sorensen_dice = 0; 
	for (int x=0;x<maxX;x++) 
	{
		for (int y=0;y<maxY;y++)
		{
			for (int z=0;z<maxZ;z++) 
			{
				if (img1_slice->Get(x,y,z) > 0) 
				{
					tot_img1++; 
					if (img2_slice->Get(x,y,z) > 0) 
					{
						tot_img1_and_img2++; 
					}
				}

				if (img2_slice->Get(x,y,z) > 0) 
				{
					tot_img2++;
				}
			
			}
		}
	}

	sorensen_dice = 100*(2*tot_img1_and_img2)/(tot_img1+tot_img2); 
	
}
double getStats(int measure, double mean)
{
	double sum=0, max=-1, t; 
	double size = i_ctr.size(); 

	if (measure == 1)			// reutnr mean 
	{
		for (int i=0;i<size;i++)
		{
			sum+=i_ctr[i]; 
		}
		return sum/size;			
	}
	if (measure == 2)			// return stdev
	{
		for (int i=0;i<size;i++)
		{
			t = i_ctr[i]; 
			sum += ((t-mean)*(t-mean));
		}
		return sqrt(sum / size);
	}
}



void ComputeSliceDiceForImages(GreyImage* img1, GreyImage* img2, ofstream& fileIO, char* appendTxt, int x_y_z) 
{
	int maxX, maxY, maxZ; 
	
	double total_error, mean, std, dice; 
	maxX = img1->GetX(); 
	maxY = img1->GetY(); 
	maxZ = img1->GetZ(); 
	
	//vector<double> total_errors; 

	GreyImage img1_crop, img2_crop; 


	if (x_y_z == 1) 
	{
		// along x-direction 
		for (int x=0;x<maxX;x++)
		{
			GetSlice(img1, img1_crop, x, 0, 0);
			GetSlice(img2, img2_crop, x, 0, 0);
			
			GetSliceDice(&img1_crop, &img2_crop, dice); 
			
			if (dice >= 0 && dice <= 100)
				i_ctr.push_back(dice); 
			
			//fileIO << x << "\t" << dice << endl;  
			//total_errors.push_back(total_error); 
		}

	}
	
	else if (x_y_z == 2) 
	{
		// along y-direction 
		for (int y=0;y<maxY;y++)
		{
			GetSlice(img1, img1_crop, 0, y, 0);
			GetSlice(img2, img2_crop, 0, y, 0);
				
			GetSliceDice(&img1_crop, &img2_crop, dice);
			if (dice >= 0 && dice <= 100)
				i_ctr.push_back(dice); 
			
			//fileIO << y << "\t" << dice << endl;  
			//total_errors.push_back(total_error); 
			
		}
	}

	else if (x_y_z == 3) 
	{

		// along z-direction 
		for (int z=0;z<maxZ;z++)
		{
			GetSlice(img1, img1_crop, 0, 0, z);
			GetSlice(img2, img2_crop, 0, 0, z);

			GetSliceDice(&img1_crop, &img2_crop, dice); 
			
			if (dice >= 0 && dice <= 100)
				i_ctr.push_back(dice); 
			   
			//total_errors.push_back(total_error); 
			
		}
	}
	
	// compute mean and std
	mean = getStats(1, -1);
	std = getStats(2, mean); 
	fileIO << std::setprecision(4) << appendTxt << "\t" << mean << "\t" << std << endl; 

}


void Binarize(RealImage* at_wall)
{
	RealPixel *p; 
	p = at_wall->GetPointerToVoxels(); 
	for (int i=0;i<at_wall->GetNumberOfVoxels();i++)
	{
		if (*p>0) 
		{ 
			*p = 1;
		}
		p++; 
	}
	//at_wall->Write("at_wall.gipl");
}


int main(int argc, char **argv)
{
	int optind;
	bool foundArgs=false;
	//GreyImage img1, img2;
	int x_y_z=1;			
	
	char* input_f1="", *input_f2="", *output_f="", *appendTxt="";
	double t1=1, mean, stdev, total_voxels, percent;

	if (argc >= 1) 
	{
		int optind=1;
		
		while ((optind < argc) && (argv[optind][0]=='-')) {
			
			string sw = argv[optind];
			
			if (sw == "-i1") {
				optind++;
				input_f1 = argv[optind]; 
				foundArgs = true;	
			}
			
			if (sw == "-i2") {
				optind++;
				input_f2 = argv[optind]; 
				foundArgs = true;	
			}
			

			else if (sw == "-o") {
				optind++;
				output_f = argv[optind]; 
			}
				
			else if (sw == "--x") 
			{
				x_y_z = 1;
				cout << "\n\nNote: Along x"; 
			}

			else if (sw == "--y") 
			{
				x_y_z = 2;
				cout << "\n\nNote: Along y"; 
			}

			else if (sw == "--z") 
			{
				x_y_z = 3;
				cout << "\n\nNote: Along z"; 
			}
			else if (sw == "-txt")
			{
				optind++;
				appendTxt = argv[optind];
			}
			
			
			optind++; 
		}
	}
	
	if (foundArgs == false)
	{
		cout << "Usage: slicedice_mean.exe -i1 <img1> -i2 <img2> -o <output dice txt> -txt <append text to file> --x --y --z <along which direction> " << endl; 
		exit(0); 
	}

	else
	{
		InitializeIOLibrary();
		//GreyImage img1(input_f1);
		//GreyImage img2(input_f2); 

		UniquePtr<BaseImage> img1(BaseImage::New(input_f1));
		UniquePtr<BaseImage> img2(BaseImage::New(input_f2));


		/*
		img1.Read(input_f1); 
		img2.Read(input_f2);*/

		// check images are same size 
		if (!(img1->GetX() == img2->GetX() && img1->GetY() == img2->GetY() && img1->GetZ() == img2->GetZ()))
		{
			cout << "Image size mismatch, images need to be of equal dimensions! " << endl; 
			exit(0); 
		}
		
		ofstream fileIO; 
		fileIO.open(output_f, std::ios_base::app); 
		
		ComputeSliceDiceForImages(new GenericImage<short>(*img1), new GenericImage<short>(*img2), fileIO, appendTxt,  x_y_z);
		fileIO.close();
	}

}
