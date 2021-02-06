/**
 * Alexander McRae
 * V00890516
 * Short description of the program
 */

import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.*;
import javax.imageio.ImageIO;

/**
 * Class ImageConversions is a collection of methods that convert jpg images
 * into variations.
 */
public class ImageConversions {
	/**
	 * Handles all input instructions and relays the information to the
	 * appropriate method.
	 * If the input is not as specified, the program stops, with an informative message.
	 * @param args The input necessary to execute the appropriate conversion.
	 * The array is formated in one of the following ways:
	 * <ul style="list-style:none;">
	 * <li>[infile outfile] invert
	 * 	<ul style="list-style:none;">
	 *	<li>infile: image file  outfile: image file</li>
	 * 	</ul></li>
	 * <li>[infile outfile] verticalFlip</li>
	 * <li>[infile outfile] horizontalFlip</li>
	 * <li>[infile outfile] makeAscii</li>
	 * <li>[infile outfile] scale [scalefactor]
	 *	<ul style="list-style:none;">
	 *	<li>scalefactor: A number: 1 maintains original size, &lt; 1 reduces size, &gt; 1 enlarges size.</li>
	 *	</ul></li>
	 * <li>[infile outfile] rotate</li>
	 * </ul>
	 */
	public static void main(String[] args) throws IOException {
		String inFile = args[0];
        String outFile = args[1];
        String action = args[2];
        int[][] pic = readGrayscaleImage(inFile);

        if (action.equals("verticalFlip")) {
            verticalFlip(pic);
        } else if (action.equals("horizontalFlip")) {
            horizontalFlip(pic);
        } else if (action.equals("invert")) {
            invert(pic);
        } else if (action.equals("rotate")) {
            pic = rotate(pic);
        } else if (action.equals("scale")) {
            pic = scale(pic, Double.parseDouble(args[3]));
        } else if (action.equals("makeAscii")) {
            makeAscii(pic, outFile);
        }
        if (!action.equals("makeAscii")) {
            writeGrayscaleImage(outFile, pic);
	   }
       System.out.println(inFile + outFile + action);
    }	
				
	// THIS METHOD MAY BE CALLED, BUT MUST NOT BE MODIFIED!
	/** 
	 * Reads an image file and converts it to a 2D array
	 * of integers.
	 * Each value in the array is a representation
	 * of the corresponding pixel's grayscale value.
	 * @param filename The name of the image file
	 * @return A 2D array of integers.
	 * @throws RuntimeException if the input file cannot be found or read.
	 */
	public static int[][] readGrayscaleImage(String filename) {
		int[][] result = null;
		File imageFile = new File(filename);
		BufferedImage image = null;
		try {
			image = ImageIO.read(imageFile);
		} catch (IOException ioe) {
			System.err.println("Problems reading file named " + filename);
			throw new RuntimeException("Please ensure the image file is saved in the same directory as your java file.");
		}
		int height = image.getHeight();
		int width  = image.getWidth();
		result = new int[height][width];
		int rgb;
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				rgb = image.getRGB(x, y);
				result[y][x] = rgb & 0xff;
			}
		}
		return result;
	}

	// THIS METHOD MAY BE CALLED, BUT MUST NOT BE MODIFIED!
	/**
	 * Reads a 2D array of integers and creates
	 * a grayscale image. Each pixel's grayscale value is
	 * based on the corresponding value in the 2D array.
	 * @param filename The name of the image file to create
	 * @param array The 2D array of integers
	 */
	public static void writeGrayscaleImage(String filename, int[][] array) {
		int width = array[0].length;
		int height = array.length;
		BufferedImage image = new BufferedImage(width, height,BufferedImage.TYPE_INT_RGB);

		int rgb;
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				rgb = array[y][x];
				rgb |= rgb << 8;
				rgb |= rgb << 16;
				image.setRGB(x, y, rgb);
			}
		}
		File imageFile = new File(filename);
		try {
			ImageIO.write(image, "jpg", imageFile);
		} catch (IOException ioe) {
			System.err.println("The file could not be created " + filename);
			return;
		}
	}
    /*
    * flips image vertically
    */
    public static int[][] verticalFlip(int [][] image) {
        int length = (image.length)/2;
        for (int i = 0; i <= length; i++) {
            int [] temp = image[i];
            image[i] = image[image.length - i -1];
            image[image.length - i - 1] = temp;
        }

        return image;
    }
    /*
    * iverts image
    */
    public static int[][] invert(int[][] image) {
        double mid = 255.0/2;
        for (int i = 0; i < image.length; i++) {
            for (int j = 0; j < image[i].length; j++) {
                double dif = mid - image[i][j];
                image[i][j] = (int)(mid + dif);
            }
        }
        return image;
    }
    /*
    * flips image horizontally
    */
    public static int[][] horizontalFlip(int[][] image) {
        for (int i = 0; i < image.length; i++) {
            int length = image[i].length/2;
            for (int j = 0; j <= length; j++) {
                int temp = image[i][j];
                image[i][j] = image[i][image[i].length - j - 1];
                image[i][image[i].length - j - 1] = temp;
            }
            
        }
        return image;
    }
    /*
    * rotates the image 90
    */
    public static int[][] rotate(int[][] image) {
        int [][] rotated = new int [image[0].length][image.length];
        for (int i = 0; i < image.length; i++) {
            for (int j = 0; j < image[i].length; j++) {
                rotated[j][i] = image[i][j];
            }
            
        }
        horizontalFlip(rotated);
        return rotated;
    }
    /*
    * Scales the image by a double
    */
    public static int[][] scale(int[][] image,
                            double scalefactor) {
        int oldHeight = image[0].length;
        int oldWidth = image.length;
        int newHeight = (int)(image[0].length * scalefactor);
        int newWidth = (int)(image.length * scalefactor);
        
        if (scalefactor == 1) {
            System.out.println("The scale was one");
            return image;
        } else if (scalefactor > 1) {
            System.out.println("The scale was bigger than one");
            return ScaleUp(image, oldHeight, newHeight, oldWidth, newWidth);
        } else if (scalefactor < 1) {
            return ScaleDown(image, oldHeight, newHeight, oldWidth, newWidth);
        }
        int ratioW = newWidth/oldWidth;
        int ratioH = newHeight/oldHeight;
        System.out.println(ratioH);
        /*
        // condense horizontally
        // create a new array with new width but same height
        int [][] heightExpand = new int [oldWidth][newHeight];

        // go threw each cell in old array making multiples and putting in the new array
        for (int i = 0; i < oldWidth; i++) {
            int count = 0;
            for (int j = 0; j < oldHeight; j++) {
                for (int r = 0; r < ratioH; r++) {
                    //System.out.println((r + count) + " is the count");
                    if ((count + r) < newHeight) {
                       heightExpand[i][r + count] = image[i][j];
                    }
                }
                count = count + ratioH;
            }
        }

        int [][] scaled = new int [newWidth][newHeight];

        for (int i = 0; i < newHeight; i++) {
            int count = 0;
            for (int j = 0; j < oldWidth; j++) {
                for (int r = 0; r < ratioW; r++) {
                    if ((count + r) < newWidth) {
                        scaled[count + r][i] = heightExpand[j][i];
                    }
                }
                count = count + ratioW;
            }
        }
        
        // do that vertically
        // put into new database
        return scaled;*/
        return new int [1][1];
    }
    /*
    * scales down image
    */
    public static int[][] ScaleDown(int[][] image, int oldh, int newh, int oldw,
                                  int neww) {
        int [][] WidthShrink = new int[oldw][newh]; // old width new height
        int ratioW = oldw/neww; // how many blocks per old block
        
        for (int i = 0; i < WidthShrink.length; i++) { //goes through each row //set to widthexpeand.length
            int count = 0;//how much the offset is at each row
            for (int j = 0; j < newh; j++) { // goes through each coloumb
                int sum = 0;
                for (int k = 0; k < ratioW; k++) { //makes a one cell in old array multiple
                    sum += image[i][k+count];
                }
                WidthShrink[i][j] = sum/ratioW;
                count = count + ratioW;

            }
        }

        int [][] heightShrink = new int[neww][newh];
        int ratioH = oldh/newh;

        for(int i  = 0; i < newh; i++) {
            int count = 0;
            for (int j = 0; j < neww; j++) {
                int sum = 0;
                for (int k = 0; k < ratioH; k++) {
                    sum += WidthShrink[k+count][j];
                }
                heightShrink[j][i] = sum/ratioH;
                count = count + ratioH;
            }
        }
        return heightShrink;
    }
    /*
    * Scales image up
    */
    public static int[][] ScaleUp(int[][] image, int oldh, int newh, int oldw,
                                  int neww) {

        int [][] WidthExpand = new int[oldw][newh]; // old width new height
        int ratioW = neww/oldw; // how many blocks per old block
        
        for (int i = 0; i < WidthExpand.length; i++) { //goes through each row //set to widthexpeand.length
            int count = 0;//how much the offset is at each row
            for (int j = 0; j < oldh; j++) { // goes through each coloumb
                for (int k = 0; k < ratioW; k++) { //makes a one cell in old array multiple
                    WidthExpand[i][k + count] = image[i][j];
                }
                count = count + ratioW;

            }
        }

        int [][] heightExpand = new int[neww][newh];
        int ratioH = newh/oldh;

        for(int i  = 0; i < WidthExpand[0].length; i++) {
            int count = 0;
            for (int j = 0; j < oldw; j++) {
                for (int k = 0; k < ratioH; k++) {
                    heightExpand[k + count][i] = WidthExpand[j][i];
                }
                count = count + ratioH;
            }
        }
        return heightExpand;
    }
    /*
    * turns image into ascii representation
    */
    public static void makeAscii(int[][] image, String outName) throws IOException {
        PrintWriter writer =  new PrintWriter(outName, "UTF-8");
        for (int i = 0; i < image[0].length; i++) {
            for (int j = 0; j < image.length; j++) {
                writer.print(convertIntToAscii(image[j][i]));
            }
            writer.println();
        }    
        writer.close();
    }
    /*
    * converts int to acsii
    */
    public static char convertIntToAscii(int num) {
        if (num <= 20 ) {
            return('M');
        } else if (num <= 40) {
            return('L');
        } else if (num <= 60) {
            return('I');
        } else if (num <= 80) {
            return('O'); //cant tell if its upper or lower case
        } else if (num <= 100) {
            return('|');
        } else if (num <= 120) {
            return('=');
        } else if (num <= 140) {
            return('*');
        } else if (num <= 160) {
            return(':');
        } else if (num <= 180) {
            return('-');
        } else if (num <= 200) {
            return(',');
        } else if (num <= 220) {
            return('.');
        } else if (num <= 255) {
            return(' ');
        }
        return '/';
    }

}
