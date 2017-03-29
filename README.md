# Segmentation for atrial wall from CT Angiography 
A simple segmentation method that segments the atrial wall using iterative dilation and intensity thresholding. It requires the intensity distribution information of the ventricular myocardium. 

## Usage 
The usage is through command line: 
```
mirtk evaluate-la-wall <input_img> <la_mask> <output_img> <intensity_t1> <intensity_t2> <iterations_n>
```

## Image requirements 
Note that the program assumes that the input images are CTA (contrast-enhanced angiography). 

This program outputs the la wall segmented from endocardial segmentation based on intensity values [t1,t2] and dilation with n steps. 

The program expects a binary mask to be provided as an input parameter ```<la_mask>```, where ```1``` is set as endocardium. This is the endocardial segmentation of the atrial blood pool. 

All input image files are either NifTII or GIPL or HDR Analyze. 

## Parameters 
the ```<intensity_t1>``` and ```<intensity_t2>``` are intensity ranges manually selected for the atrial wall. The values ```t1``` and ```t2``` are ideally obtained from ventricular myocardium, +/- 2-standard deviations of the mean of an intensity sample from that region. 

## Output 
The program outputs a binary image with the wall labelled sequentially at each iterative step as 1, 2 and so on. 

## Author 
```
Dr. Rashed Karim 
Department of Biomedical Engineering 
King's College London 
```
