/*
* Name: Alexander McRae
* ID: V00890516
* Date: 9/19/2017
* Filename: FirstProgram.java
* Details: \CSC 110\ Assignment 5
*/

/*
* The class DNASequencing contains methods for
* printing arrays, finding the longest and 
* shortest string in a array, finding the
* frequency of a string in a array with and 
* without mutations, a method to get rid of
* the mutation and a method to count mutations
*/
public class DNASequencing {
    /*
    * An internal tester to check the methods.
    */
    public static void main(String[] args) {
        String [] testArray = {"AC","TACG","AAC","GTT"};
        String [] testArray2 = {"TG","TGA","ACT","GA","CCT"};
        printArray(testArray);
        System.out.println("The longest string is " + findLongest(testArray));
        System.out.println("The longest string is " + findLongest(testArray2));
    }

    /*
    * Prints out the contents of an array, one item per line
    */
    public static void printArray(String[] strings) {
        System.out.println("The array contains the following:");
        for (String cell: strings) {
            System.out.print(cell + "\n");
        }
        System.out.println();
    }

    /*
    * Finds the first occurrence of the longest item in an array of String 
    * objects. The length of a String object is determined by its number of 
    * characters
    */
    public static String findLongest(String[] strings) {
        String longest = "";
        for (String i: strings) {
            if (i.length() > longest.length()) {
                longest = i;
            }
        }
        return longest;
    }

    /*
    * Determines the number of times a particular item occurs within an array 
    * of String objects
    */
    public static int findFrequency(String target,
                                String[] strings) {
        int count = 0;
        for (String i: strings) {
            if (target.equals(i)) {
                count++;
            }
        }
        return count;
    }

    /*
    * Determines the number of times a particular DNA sequence occurs within
    * an array of DNA sequences. A typical DNA sequence contains a sequence of
    * two to four of the following characters: {A,C,G,T}. A mutated sequence 
    * contains adjacent repeated characters , so TCA, TTCCCCA, and TCCAA are 
    * all equivalent, TTCCCCA and TCCAA being mutations of TCA.
    */
    public static int findFreqWithMutations(String target,
                                        String[] seqArray) {
        int count = 0;
        for (String i: seqArray) {
            i = getRidOfMutations(i);
            if (target.equals(i)) {
                count++;
            }
        }
        return count;
    }

    /*
    * Converts mutated sequences back into their normal sequences
    */
    public static String getRidOfMutations (String sequence) {
        String newString = "";
        newString += sequence.charAt(0);
        for (int i = 1; i < sequence.length(); i++) {
            if (sequence.charAt(i) != sequence.charAt(i-1)) {
                newString += sequence.charAt(i);
            }

        }
        return newString;
    }

    /*
    * Counts the number of mutated DNA sequences that occur within an array.
    * Note that DNA sequences contain two to four of the following characters:
    * {A,C,G,T} in any order. A mutated sequence contains adjacent repeated 
    * characters , so ACCG, TTCCA, and AACC are all mutated sequences.
    */
    public static int countTotalMutations(String[] seqArray) {
        int count = 0;
        for (String i: seqArray) {
            if (hasMutation(i)){
                count++;
            }
        }
        return count;
    }

    /*
    * Determines if a given DNA sequence has a mutation. A mutated sequence
    * contains adjacent repeated characters , so ACCG, TTCCA, and AACC are all
    * mutated sequences.
    */
    public static boolean hasMutation(String sequence) {
        for (int i = 1; i < sequence.length(); i++) {
            if (sequence.charAt(i) == sequence.charAt(i-1)) {
                return true;
            }
        }
        return false;
    }



}