class PostfixTokenizer implements Tokenizer {
    private Node Head;
    private Node curr;
    private int currIdex = 0;

    /*
    * Testing
    */
    public static void main(String[] args) {
        String asString = "3*(4+-2)";
        OperatorTokenizer asInfix = new OperatorTokenizer(asString);
        System.out.println("Testing the postFix constructor");
        PostfixTokenizer postfix = new PostfixTokenizer(asInfix);
        System.out.println("Done testing postfix constructor");
        System.out.print("as infix tokens: ");
        System.out.println(asInfix);
        System.out.println("By individual postfix tokens:");
        while(postfix.hasNext()) {
            System.out.println("next token: "+postfix.next());
        }
    }

    /*
     * Takes the infix tokenizer and converts it into the postfix version
     */
    public PostfixTokenizer(OperatorTokenizer infixTokens)
                 throws IllegalExpressionException {
        //Creates a stack for the operators
        StringStack aStack = new StringStack();
        
        while(infixTokens.hasNext()) {
            String ch = infixTokens.next();
            if (ch.equals("(")) {
                aStack.push(ch);
            } else if (ch.equals(")")) {
                while (!aStack.isEmpty() && !aStack.peek().equals("(")){
                    if (this.Head != null) {
                        this.addNode(aStack.pop());
                    } else {
                        throw new IllegalExpressionException("tried to add without a head");
                    }
                }
                if (aStack.isEmpty()){
                    throw new IllegalExpressionException("Missing '('");
                } else {
                    aStack.pop();
                }
            } else if (Operator.isOperator(ch)) {
                while (!aStack.isEmpty() && !aStack.peek().equals("(") &&
                        Operator.comparePrecedence(ch, aStack.peek()) >= 0) {
                            if (this.Head != null){
                                this.addNode(aStack.pop());
                            } else {
                                throw new IllegalExpressionException("tried to add without head");
                            }
                        }
                aStack.push(ch);
            } else if (!Operator.isOperator(ch)) {
                if (this.Head != null){
                    this.addNode(ch);
                } else {
                    this.Head = new Node(ch);
                    this.curr = this.Head;
                    this.currIdex = 0;
                }
            }
        }
        while (!aStack.isEmpty()){ 
            if (!aStack.peek().equals("(")) {
                this.addNode(aStack.pop());
            } else {
                throw new IllegalExpressionException("Missing atleast 1 ')'");
            }
        }
        this.reset();
    }

    private void addNode(String a) {
        this.curr.setNext(new Node(a));
        this.curr = this.curr.getNext();
        this.currIdex++;
    }

    /*
     * Sets the iterator start position for the first item in the list
     */
    public void reset() {
        this.curr = this.Head;
        this.currIdex = 0;
    }

    /*
     * The number of tokens in the list
     */
    public int numTokens() {
        Node tmp = this.curr;
        int tmp2 = this.currIdex;
        this.reset();
        int count = 0;
        while(this.curr.getNext() != null) {
            this.next();
            count++;
        }
        this.curr = tmp;
        this.currIdex = tmp2;
        return count;
    }

    /*
     * Provides a string representation of the all the elements in the list,
     * from first to last
     */
    public String toString() {
        Node temp = this.curr;
        int tmp2 = this.currIdex;
        this.reset();
        String tmp = "";
        while(this.hasNext()) {
            tmp += this.curr.getItem();
            this.curr = this.curr.getNext();
        }
        this.curr = temp;
        this.currIdex = tmp2;
        return tmp;
    }

    /*
     * Returns true if the iteration has more elements
     */
    public boolean hasNext() {
        return this.currIdex <= this.numTokens();
    }

    /*
     * Returns the next element in the iteration
     */
    public String next() {
        String tmp = this.curr.getItem();
        this.curr = this.curr.getNext();
        this.currIdex++;
        return tmp;
    }

    /*
     * Removes from the underlying collection the last element returned by this
     * iterator (optional operation)
     */
    public void remove() {
        Node tmp = this.curr.getNext().getNext();
        this.curr.getNext().setNext(null);
        this.curr.setNext(tmp);
    }

}