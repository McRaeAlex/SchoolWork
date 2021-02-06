class VigenereCipher implements Cipher {
    private String key;

    /*
    * Testing
    */
    public static void main(String[] args) {
        int [] arr = {1,2,3,4,5,6,7,8};
        VigenereCipher c = new VigenereCipher("abcdefghi");
        //c.dumpArray(arr, "THis is balasdf");
        //System.out.println();
        //arr = c.stringToIntArray("abcdefghijklmnopqrstuvwxyz");
        //for (int i : arr) {System.out.println(i);}
        //System.out.println(c.intArrayToString(arr));
        /*System.out.println("abcdefghijklmnopqrstuvwxyz");
        String encrypted = c.encrypt("abcdefghijklmnopqrstuvwxyz");
        System.out.println(encrypted);
        String decrypted = c.decrypt(encrypted);
        System.out.println(decrypted);*/
    }

    /*
    * Creates a VigenereCipher object.
    */
    public VigenereCipher(String key) {
        setKey(key);
    }

    /*
    * Prints out the specified text, followed immediately by the
    * (comma-delimited) contents of the array.
    */
    private void dumpArray(int[] array,
                         String text) { //done
        System.out.print(text);
        if (array.length != 0) {
            System.out.print(array[0]);
            for (int i = 1; i < array.length; i++) {
                System.out.print("," + array[i]);
            }
        }
    }

    /*
    * Converts a string into an int array where the values are within the range
    * 0...25. The values are matched, in order, to the characters in the string.
    *  For example, the integer 0 is matched to the letter 'a' and 25 is matched
    *  to and 'z'.
    */
    private int[] stringToIntArray(String text) { //done
        int [] arr = new int[text.length()];
        for (int i = 0; i < text.length(); i++) {
            arr[i] = ((int)text.charAt(i)) - 97;
        }
        return arr;
    }

    /*
    * Converts an array of integers with values in the range 0...25 into a
    * string with characters in the range a...z. The individual letters are
    * ordered exactly as the corresponding integer values. For example, the
    * value 0 in index position i of the array, matches an 'a' as the first
    * letter in the string.
    */
    private String intArrayToString(int[] encodedText) { //done
        String str = ""; 
        for (int i = 0; i < encodedText.length; i++) {
            str += (char)(encodedText[i] + 97);
        }
        return str;
    }

    /*
    * Encrypts a string using a simplified Vigenere cipher.
    * All text is limited to lower-case ASCII letters a...z
    */
    public String encrypt(String plaintext) {
        int [] arr = stringToIntArray(plaintext);
        String keyS = "";
        String str = "";
        while(keyS.length() < plaintext.length()) {keyS += this.key;}
        int [] key = stringToIntArray(keyS); 
        for(int i = 0; i < arr.length; i++) {
            str += (char)(((key[i] + arr[i]) % 26) + 97);
        }
        return str;
    }

    /*
    * Decrypts a string using a modified Vigenere cipher.
    * All text is limited to lower-case ASCII letters a...z
    */
    public String decrypt(String ciphertext) {
        int [] arr = stringToIntArray(ciphertext);
        String keyS = "";
        String str = "";
        while(keyS.length() < ciphertext.length()) {keyS += this.key;}
        int [] key = stringToIntArray(keyS); 
        for(int i = 0; i < arr.length; i++) {
            str += (char)(((26 + arr[i] - key[i])%26) + 97);
        }
        return str;
    }

    /*
    * Sets the key for a simplified Vigenere cipher.
    * All text is limited to lower-case ASCII letters a...z
    */
    public void setKey(String key) {
        this.key = key;
    }
}