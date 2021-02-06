/*
* Name: Alexander McRae
* ID: V00890516
* Date: 10/07/2017
* Filename: CalendarGames.java
* Details: \CSC 110\ Assignment4
*/

/*
* The class CalendarGames contains methods for
* converting months to numbers and numbers to
* months, checking if its a leap year, 
* play a interactive game, and getting the days
* in a month
*/
import java.util.Scanner;

public class CalendarGames{
    public static int count = 0; // i got lazy and didnt want to pass it
    
    /*
    * Converts a numeric version of a month into its full name.
    */
    public static String monthToString(int month) {
        String [] Months = {"January", "Febuary", "March", "April", "May",
                            "June", "July", "August", "September", "October",
                            "Novemeber", "December"};

        if ( month>0 && month<13){
            return(Months[month-1]);
        } else {
            return("Not a valid month");
        }
    }

    /*
    * Determines whether a given year is a leap year
    */
    public static boolean isLeapYear(int year) {
        if (year%4 == 0 && (year%100 != 0 || year%400 == 0)) {
            return(true);
        } else {
            return(false);
        }
    }

    /*
    *  Determines the number of days in the gicen month
    */
    public static int numDaysInMonth(int month, int year) {
        int [] monthDays = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (month == 2) {
            if (isLeapYear(year)) {
                return (29);
            }
            else return(28);
        } else {
            return(monthDays[month-1]);
        }
    }

    /*
    * A simple interactive computer-guessing game
    */
    public static void guessMyBirthday() {
        Scanner scanner = new Scanner(System.in);
        String month = getMonth(scanner);
        int date = getDate(scanner, month);
        System.out.printf("It took %d guesses to find your birthday, "
                        + "which is %s %d", count, month, date);
    }

    /*
    * uses a binary search algorithim to find the users
    * month of birth
    */
    public static String getMonth(Scanner scanner) {
        int low = 1;
        int high = 12;
         
        while(high >= low) {
             int middle = (low + high) / 2;
             String month = monthToString(middle);
             System.out.printf("Is your birthday in %s? yes or no: ", month);
             count++;
             if(scanner.next().equals("yes")) {
                 return month;
             }
             System.out.print("Is your birthday after this month? yes or no: ");
             String tmp = scanner.next();
             if(tmp.equals("yes")) {
                 low = middle + 1;
             }
             if(tmp.equals("no")) {
                 high = middle - 1;
             }
       }
        
        return "You lied";
    }

    /*
    * uses a binary search algorithim to ask find the users
    * date of birth
    */
    public static int getDate(Scanner scanner, String month) {
        int low = 1;
        int high = numDaysInMonth(monthToInt(month),1);
         
        while(high >= low) {
             int middle = (low + high) / 2;
             System.out.printf("Is your birthday in %s %d? yes or no: ", 
                            month, middle);
             count++;
             if(scanner.next().equals("yes")) {
                 return middle;
             }
             System.out.print("Is your birthday after this day? yes or no: ");
             String tmp = scanner.next();
             if(tmp.equals("yes")) {
                 low = middle + 1;
             }
             if(tmp.equals("no")) {
                 high = middle - 1;
             }
       }
       return -1;
    }

    /*
    * Converts name of month to int
    */
    public static int monthToInt(String month){
        String [] Months = {"January", "Febuary", "March", "April", "May",
                            "June", "July", "August", "September", "October",
                            "Novemeber", "December"};
        for (int i = 0; i < Months.length; i++) {
            if (Months[i].equals(month)){
                return (i+1);
            }
        }
        return -1;
    } 

    public static void main(String[] args) {

    }
}