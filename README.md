# Dice Similarity Index for 3D medical image slices
Finds the [Sorensen dice](http://en.wikipedia.org/wiki/Sørensen–Dice_coefficient) of two 3D (medical) image binary masks. 

The slices can be defined by the user using the --x, --y or, --z switches

However, it requires post-processing for finalising the atrial wall segmentation. The following repository contains two separate tools that accomplish these post-processing tasks. 

## Usage 
The usage for the first tool ```combinmask``` is:
```
./slice_dice -i1 <img1> -i2 <img2> -o <output dice txt> -txt <append text> --x --y --z <along which direction>
```
This program outputs the mean and standard deviation of dice in each slice along the specified X, Y, Z direction. 



## Author 
```
Dr. Rashed Karim 
Department of Biomedical Engineering 
King's College London 
```
