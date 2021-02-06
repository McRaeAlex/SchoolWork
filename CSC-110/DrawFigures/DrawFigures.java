public class DrawFigures {
	public static void main(String[] args) {
		drawDimond();
		drawX();
		drawRocket();
	}

	public static void drawDimond() {
		drawCone();
		drawV();
		System.out.println();
	}

	public static void drawX() {
		drawV();
		drawCone();
		System.out.println();
	}

	public static void drawRocket() {
		drawCone();
		drawBox();
		System.out.println("|Canada|");
		System.out.println("|  Eh! |");
		drawBox();
		drawCone();
		System.out.println();
	}

	public static void drawBox() {
		System.out.println("+------+");
		System.out.println("|      |");
		System.out.println("|      |");
		System.out.println("+------+");
	}

	public static void drawCone() {
		System.out.println("   /\\");
		System.out.println("  /  \\");
		System.out.println(" /    \\");
	}

	public static void drawV() {
		System.out.println(" \\    /");
		System.out.println("  \\  /");
		System.out.println("   \\/");
	}
}