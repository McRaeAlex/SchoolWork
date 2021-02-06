class StringStack { //done
    private Node top;

    /*
    * Testing Only
    */
    public static void main(String[] args) {
        StringStack stack = new StringStack();
        System.out.println("Testing isEmpty Method: Should return True");
        System.out.println("Returns: " + stack.isEmpty());
        System.out.println("Testing push Method: isEmpty should return False");
        stack.push("Test");
        System.out.println("Returns: " + stack.isEmpty());
        System.out.println("Testing pop Method: isEmpty should return True");
        System.out.println("Testing pop Method: pop should return 'test'");
        System.out.println("Returns: " + stack.pop() + " " + stack.isEmpty());
        stack.push("test2");
        System.out.println("Testing peek Method: peek should return 'test2'");
        System.out.println("Returns: " + stack.peek());
        stack.push("test3");
        System.out.println("Testing popAll Method: isEmpty should return true");
        stack.popAll();
        System.out.println("Returns: " + stack.isEmpty());
        System.out.println("Should return an error");
        stack.pop();
    }

    /*
    * Initializes a stack that has no elements
    */
    public StringStack () { //done
        this.top = null;
    }

    /*
    * True if the stack is empty, false if not
    */
    public boolean isEmpty() { //done
        if (this.top == null) {
            return true;
        } else {
            return false;
        }
    }

    /*
    * Inserts an element onto the top of the stack.
    */
    public void push(String s) { //done
        this.top = new Node(s, this.top);
    }

    /*
    * Removes the top element from the stack
    */
    public String pop() { //done
        if (this.isEmpty()) {
            throw new StackException("not enough operands");
        } else {
            Node tmp = this.top;
            this.top = this.top.getNext();
            return tmp.getItem();
        }
    }

    /*
    * Accesses the top element of the stack, without removing it
    */
    public String peek() throws StackException { //done
        if (this.isEmpty()){
            throw new StackException("Stack empty");
        }
        return this.top.getItem();
    }

    /*
    * Empties all the elements from the stack.
    */
    public void popAll() { //done
        this.top = null;
    }
}