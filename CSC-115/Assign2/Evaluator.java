class Evaluator {
    public static void main(String[] args) {
        String good = "3*(4+2)-4*(-6--3)";
        String badParens = "(4))";
        String toomanyOperands = "(4+6)12";
        String notEnough = "3--6+2*";
        System.out.println(good);
        System.out.println(evaluate(good));

        System.out.println("BadParens: " + badParens);
        try {
            evaluate(badParens);
        } catch (IllegalExpressionException iee) {
            System.out.println(iee.getMessage());
        }

        System.out.println("ToManyOperands: " + toomanyOperands);
        try {
            evaluate(toomanyOperands);
        } catch (IllegalExpressionException iee) {
            System.out.println(iee.getMessage());
        }

        System.out.println("Not enough operands: "+notEnough);
        try {
            evaluate(notEnough);
        } catch (IllegalExpressionException iee) {
            System.out.println(iee.getMessage());
        }
        
    }

    public static double evaluate(String expression) {
        PostfixTokenizer tokenizer = new PostfixTokenizer(new OperatorTokenizer(expression));
        StringStack stack = new StringStack();
        while (tokenizer.hasNext()) {
            String s = tokenizer.next();
            if (!Operator.isOperator(s)){
                stack.push(s);   
            } else {
                try {
                    double op2 = Double.parseDouble(stack.pop());
                    double op1 = Double.parseDouble(stack.pop());
                    double result = Operator.evaluate(op1, op2, s);
                    stack.push(Double.toString(result));
                } catch (StackException e) {
                    throw new IllegalExpressionException("Not enough operands");
                } 
            }
        }
        //checks if there is more than one thing left on the stack
        String r = "";
        int count = 0;
        while (!stack.isEmpty()) {
            r = stack.pop();
            count++;
        }
        if (count > 1) {
            throw new IllegalExpressionException("To many operands");
        } else if (!Operator.isOperator(r)){
            return Double.parseDouble(r);
        } else {
            throw new IllegalExpressionException("Operator still left on stack");
        }
    }
}