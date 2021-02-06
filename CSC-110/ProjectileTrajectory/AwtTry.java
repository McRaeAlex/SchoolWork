import java.awt.*;

class AwtTry {
    public static void main(String[] args) {
        DrawingPanel panel = new DrawingPanel(100,100);
        panel.setBackground(Color.CYAN);

        Graphics g = panel.getGraphics();
        for (int i = 0; i < 10; i++) {
            g.drawRect(i *10, i *10, 50, 25);
            g.setColor(Color.WHITE);
            g.fillOval(i*10,i*10,50,25);
        }
    }
}