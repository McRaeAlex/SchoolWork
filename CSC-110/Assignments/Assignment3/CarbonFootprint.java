/*
* Name: Alexander McRae
* ID: V00890516
* Date: 10/07/2017
* Filename: CarbonFootprint.java
* Details: \CSC 110\ Assignment3
*/

/*
* The class CarbonFootprint contains methods for
* calculating carbon footprint with respect to
* different aspects of life
*/

import java.util.Scanner; // imports the scanner object

public class CarbonFootprint {

    /*
    * Creates a scanner object and get user input
    * take Users that input and gives it to other
    * methods
    */
    public static void main(String[] args) {
        Scanner console = new Scanner(System.in);
        System.out.print("How many km do you drive in one day? ");
        double kpd = console.nextDouble();
        System.out.print("Choose the appropriate fuel efficency for your car:\n"
                        + "\tvery small car : 7.1 km/litre\n"
                        + "\tsmall car : 8.2 km/litre\n"
                        + "\tsports car : 14.9 km/litre\n"
                        + "\tSUV : 12.3 km/litre\n"
                        + "Or, if you know the efficency rating, choose another number: ");
        double effic = console.nextDouble();
        double TransFootPrint = transportationFootprint(kpd, effic);
        System.out.printf("Your Carbon footprint with respect to car use is %skg/year.\n\n", Double.toString(TransFootPrint));

        System.out.print("What is the average electricity consumption per month, in kilowatts? ");
        double electconsump = console.nextDouble();
        System.out.print("How many people live in the house? ");
        int people = console.nextInt();
        double avgElectricConsumption = electricityFootprint(electconsump, people);
        System.out.printf("Your Carbon footprint with respect to electricity use is %skg/year.\n\n",
                        Double.toString(avgElectricConsumption));

        System.out.print("Of all the food you eat, what percent consists of meat or fish? ");
        double meat = console.nextDouble();
        System.out.print("Of all the food you eat, what percent consists of dairy? ");
        double dairy = console.nextDouble();
        System.out.print("Of all the food you eat, what percent consists of fruit of vegetables? ");
        double fruit = console.nextDouble();
        System.out.print("Of all the food you eat, what percent consists of carbohydrates? ");
        double carbs = console.nextDouble();
        double foodConsumption = foodFootprint(meat, dairy, fruit, carbs);
        System.out.printf("Your Carbon Footprint with respect to food consumption is %skg/year.\n\n",
                        Double.toString(foodConsumption));

        double total = totalFootprint(TransFootPrint, avgElectricConsumption, foodConsumption);
        System.out.printf("You produce an annual total of %s metric tons of CO2 per year.",
                        Double.toString(total)); //converts to get rid of decimal
    }

    /*
    * calculates carbon footprint with respect to transportation
    */
    public static double transportationFootprint(double kPD, double effic) {
        return (2.3 * (365 * (kPD/effic)));
    }

    /*
    * calculates carbon footprint with respect to electricity
    */
    public static double electricityFootprint(double kwHours, int num) {
        return ((kwHours * 12 * 0.257)/num);
    }

    /*
    * calculates carbon footprint with respect to food
    */
    public static double foodFootprint(double meat, double dairy, double fv, double carb) {
        return (meat * 53.1 + dairy * 13.8 + fv * 7.6 + carb* 3.1)/100;
    }

    /*
    * calculates total carbon footprint
    */
    public static double totalFootprint(double trans, double elec, double food) {
        return (trans + elec + food)/1000;
    }

}