import java.util.Arrays;

public class ArrayPractive {
    public static void main(String[] args) {
        int [] x = {1,2,3,4,5,6,7};
        int [] y = x;
        int z = x[2];
        y[2] = x[4];
        System.out.println(Arrays.toString(y));
        System.out.println(Arrays.toString(x));
        System.out.println(z);
    }
}