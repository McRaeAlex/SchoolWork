/*
* Name: Alexander McRae
* ID: V00890516
* Date: 9/25/2017
* Filename: SecondProgram.java
* Details: \CSC 110\ Assignment 1
*/


import java.util.Scanner; //imports the scanner so we can use it


/*
* The class SecondProgram contains methods for
* Drawing Owls, lines, textboxes, and to approximate
* pi
*/
public class SecondProgram {
    public static void main(String[] args) {
        printOwl();
        areaCircle();
    }

    /*
    * Creates a scanner object and get user input
    * take Users that input and gives it to other
    * methods and draws a box around the user input
    */
    public static void printMessage() {
        Scanner scan = new Scanner(System.in);
        System.out.print("What does the wise owl say?  ");
        String temp = scan.nextLine();
        printLine(temp);
        System.out.print("| ");
        System.out.print(temp);
        System.out.println(" |");
        printLine(temp);

    }
    /*
    * prints a line of * where the parameter contains a
    * string that will be used for its length
    */
    public static void printLine(String userEntered) {
        for (int i = 0; i < userEntered.length() + 4; i++) {
            System.out.print("*");
        }
        System.out.println();
    }

    /*
    * Draws an ascii owl to the command line along with 
    * a line above its head. This method also calls
    * printMessage()
    */
    public static void printOwl() {
        printMessage();
        System.out.println("      \\");
        System.out.println("       \\");
        System.out.println("          ^...^");
        System.out.println("         / o,o \\");
        System.out.println("         |):::(|");
        System.out.println("        ===w=w===");
    }

    /*
    * Approximates the area of a circle
    */
    public static void areaCircle() {
        double pi = approxPi();
        Scanner math = new Scanner(System.in);
        System.out.print("Give a radius of a circle: ");
        double radius = math.nextDouble();
        double area = pi*(radius * radius);
        System.out.print("The area of a circle with radius " + radius
            + " is " + area);
    }

    /*
    * Approximates the value of pi
    * Returns: Approximation of pi as a double
    */
    public static double approxPi () {
        double nextTerm = 1; //only one nextTerm or denom has to be a double
        double denom = 1;
        double series = 0;
        for (int i = 0; i < 1000000; i++) {
            series = series + (nextTerm/denom);
            denom = denom + 2;
            nextTerm = (nextTerm * -1);
        }
        series = (series * 4);
        return series;
    }
}