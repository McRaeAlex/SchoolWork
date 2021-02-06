public class BMICalculator2 {
    public static void main(String[] args) {
        //declare variables
        double height;
        double weight;
        double bmi;

        //computer BMI
        height = 70;
        weight = 195;
        bmi = weight / (height * height) * 703;

        //print results
        System.out.println("Previouos BMI:");
        System.out.println(bmi);

        //recompute BMi
        weight = 180;
        bmi = weight / (height * height) * 703;

        //print new results
        System.out.println("Previouos BMI:");
        System.out.println(bmi);
    }
}