public class HourGlass {
    public static void main(String[] args) {
        drawLine();
        drawTop();
        drawBottom();
        drawLine();
    }

    public static void drawLine () {
        System.out.print("+");
        for (int i = 0; i < 6; i++) {
            System.out.print("-");
        }
        System.out.println("+");
    }

    public static void drawTop() {
        int circles = 4;
        int slashSpace = 0;
        for (int i = 0; i < 3; i++) {
            System.out.print("|");
            printMulti(" ", slashSpace);
            System.out.print("\\");
            printMulti(".", circles);
            circles = circles - 2;
            System.out.print("/");
            printMulti(" ", slashSpace);
            System.out.println("|");
            slashSpace++;
        }
    }

    public static void drawBottom() {
        int circles = 0;
        int slashSpace = 2;
        for (int i = 0; i < 3; i++) {
            System.out.print("|");
            printMulti(" ", slashSpace);
            System.out.print("/");
            printMulti(".", circles);
            circles = circles + 2;
            System.out.print("\\");
            printMulti(" ", slashSpace);
            System.out.println("|");
            slashSpace--;
        }
    }

    public static void printMulti(String chr, int value) {
        for (int j = 0; j < value; j++) {
                System.out.print(chr);
            }
    }
}