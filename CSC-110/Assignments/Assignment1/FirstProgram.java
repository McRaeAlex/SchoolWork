/*
* Name: Alexander McRae
* ID: V00890516
* Date: 9/19/2017
* Filename: FirstProgram.java
* Details: \CSC 110\ Assignment 1
*/

/*
* The class FirstProgram contains methods for
* drawing Owls and Frogs and also a method to
* approximate pi
*/


public class FirstProgram {

    public static void main(String[] args) {
        drawOwl();
        System.out.println(); //it says add a line or two not sure which
        drawFrog();
        approxPi();
    }

    /*
    * Draws a Ascii Frog to the console
    */
    public static void drawFrog () {
        System.out.println("  @..@");
        System.out.println(" (----)");
        System.out.println(" (>__<)");
        System.out.println("\"\"    \"\"");
    }

    /*
    * Draws a Ascii Owl to the console
    */
    public static void drawOwl () {
        System.out.println("  ^...^");
        System.out.println(" / o,o \\");
        System.out.println(" |):::(|");
        System.out.println("===w=w===");
    }

    /*
    * Approximates pi and the outputs the approx to the console
    */
    public static void approxPi () {
        double nextTerm = 1; //only one nextTerm or denom has to be a double
        double denom = 1;
        double series = 0;
        for (int i = 0; i < 8; i++) {
            series = series + (nextTerm/denom);
            denom = denom + 2;
            nextTerm = (nextTerm * -1);
        }
        series = (series * 4);
        System.out.print("My estimate for Pi is " + series); //it shows a print not println
    }
}