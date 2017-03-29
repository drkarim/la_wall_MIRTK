/****************************************************************
*      Author: 
*      Dr. Rashed Karim 
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk  
*      Copyright (c) 2016
*****************************************************************/

// Makes use of the proprietory Image Registration Toolkit library (IRTK) from Imperial Colllege London. 
#include "mirtk/Common.h"
#include "mirtk/Options.h"
#include "mirtk/IOConfig.h"
#include "mirtk/GenericImage.h"

// Other standard C libraries 
#include <fstream>
#include <vector>

using namespace mirtk;

int main(int argc, char **argv)
{
	
       char *in_file, *out_file, *la_mask_file;
       double t1, t2, iterations;
       bool skip_neigbours; 
       
       int maxX, maxY, maxZ, maxX2, maxY2, maxZ2;
       int is_debug_mode;  
       
        RealPixel *ptr1, *ptr2;
       
       short*** out_img; 
		
		if (argc < 7) 
		{
			cout << "\nUsage: la_wall <input_img> <la_mask> <output_img> <intensity_t1> <intensity_t2> <iterations_n> <debug_mode>\n\nThis program outputs" 
            "the la wall segmented from endocardial segmentation based on intensity values [t1,t2] and dilation with n steps" << endl;
			exit(1); 
		}

       // get input parameters 
       in_file = argv[1];
       argv++;
       argc--;
       la_mask_file = argv[1]; 
       argv++; 
       argc--; 
       out_file = argv[1]; 
       argv++; 
       argc--; 
       t1 = atof(argv[1]); 
       argv++; 
       argc--; 
       t2 = atof(argv[1]); 
       argv++; 
       argc--; 
       iterations = atoi(argv[1]); 
       argv++; 
       argc--; 
       is_debug_mode = atoi(argv[1]); 
       argv++; 
       argc--; 
       
       
       // read image
       InitializeIOLibrary();
       UniquePtr<BaseImage> img1(BaseImage::New(in_file));
	   UniquePtr<BaseImage> img2(BaseImage::New(la_mask_file));

       RealImage in_img; 
       GreyImage la_mask_img;
       
       la_mask_img = img2; 
       in_img = img1;

       in_img.Read(in_file);
       la_mask_img.Read(la_mask_file);
        
       maxX = in_img.GetX(); maxX2 = la_mask_img.GetX(); 
       maxY = in_img.GetY(); maxY2 = la_mask_img.GetY(); 
       maxZ = in_img.GetZ(); maxZ2 = la_mask_img.GetZ(); 
       
       if (maxX != maxX2 && maxY != maxY2 && maxZ != maxZ2)
       {
           cerr << "The dimension of the input image and input masks is a mismatch\n" << endl;
            exit(1);
       }
       
       // initialize output image
       int i,j,k,a,b,c; 
       out_img= (short***) malloc (maxX*sizeof(short**));
	   for (i=0;i<maxX;i++) out_img[i]= (short**) malloc (maxY*sizeof(short*));
	   for (i=0;i<maxX;i++) for (j=0;j<maxY;j++) out_img[i][j]= (short*) malloc (maxZ*sizeof(short));
       
       // Set output image pixels to 0 
       for (i=0;i<maxX;i++) for (j=0;j<maxY;j++)  for (k=0;k<maxZ;k++) {
           
           // copy la mask
           short v = la_mask_img.Get(i,j,k);    
           out_img[i][j][k] = v; 
           
       }
       
        
       cout << "Initialize data structures, starting image analysis .. " << endl;
       
       short this_dilation_label, prev_dilation_label; 
       
       this_dilation_label = 0; 
       prev_dilation_label = 1; 
       
       for (int dilations=0;dilations < iterations;dilations++) {
            
            this_dilation_label = 1+prev_dilation_label;
        
            // iterate through the input image 
            for (i=0;i<maxX;i++)
            {
                for (j=0;j<maxY;j++)
                {
                    for (k=0;k<maxZ;k++)
                    {
                        // iterate through neighbours 
                        skip_neigbours = false; 
                        for (a=-1;a<1 && !skip_neigbours;a++)
                        {
                            for (b=-1;b<1 && !skip_neigbours;b++)
                            {
                                for (c=-1;c<1 && !skip_neigbours;c++)
                                {
                                    if (i+a >= 0 && i+a < maxX && j+b >= 0 && j+b < maxY && k+c >= 0 && k+c < maxZ)
                                    {
                                        short this_pixel = out_img[i][j][k];
                                                                                
                                        if (this_pixel == 0) {
                                            //double neighbour = in_img.Get(i+a, j+b, k+c);
                                            short neighbour = out_img[i+a][j+b][k+c]; 
                                            if (neighbour == prev_dilation_label)
                                            {
                                                // check for intensity [t1,t2]
                                                double val = in_img.Get(i,j,k);
                                                
                                                if (dilations == 0)
                                                {
                                                    out_img[i][j][k] = this_dilation_label;
                                                } 
                                                else if (val >= t1 && val <= t2)            
                                                {
                                                    out_img[i][j][k] = this_dilation_label;    
                                                }
                                                
                                                skip_neigbours = true;
                                            }
                                        }
                                    }       // end if
                                }
                            }
                        } // end searching immediate neighbours  
                    }
                }
            }   // end image iteration loop 
            
            prev_dilation_label = this_dilation_label;      // advance the label
            
       }  // end dilation iteration loop 
       
       if (is_debug_mode <= 0)
       {
           // Fix the output so that only a binary image is output 
           for (i=0;i<maxX;i++) for (j=0;j<maxY;j++)  for (k=0;k<maxZ;k++) {
               short v = out_img[i][j][k]; 
               
               if (v==1)
                out_img[i][j][k] = 0; 
               else if (v>1)
                out_img[i][j][k] = 1;
           }
       }
       
       cout << "Completed segmenting atrial wall, now writing to output image .. " << endl;
       // write the output array to an image 
       GreyImage out_img2;
       //out_img2 = irtkGreyImage(maxX, maxY, maxZ);
       out_img2 = la_mask_img; 
       
        for (i=0;i<maxX;i++) for (j=0;j<maxY;j++)  for (k=0;k<maxZ;k++) {
            
            out_img2.Put(i,j,k, out_img[i][j][k]);
        }
        
        out_img2.Write(out_file);
       
}