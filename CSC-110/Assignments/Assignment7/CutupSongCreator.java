/*
* Name: Alexander McRae
* ID: V00890516
* Date: Nov 11, 2017
* Filename: CutupSongCreator.java
* Details: \course\ Assignment <number>
*/

import java.util.*;
import java.io.*;

/*
* CutupSongCreator is a class that handles a collection of SongLines.
*/
public class CutupSongCreator {

    /*
    * Creates an array of SongLines from a set of lines, sourced by the Scanner
    * object. These lines have the following format:
    * The first line is a single integer that indicates how many lines of
    * information will follow.
    * Each subsequent line contains the following information in order:
    * <genre>: a single word
    * <line number>: a single integer
    * <single lyric line>: all the remaining words up to the newline character.
    */
    public static SongLine[] makeArray(Scanner reader) {
        int numLines = Integer.parseInt(reader.nextLine());
        SongLine [] Songlines = new SongLine[numLines];
        for (int i = 0; i < Songlines.length && reader.hasNextLine(); i++) {
            String [] tmp = reader.nextLine().split("\t");
            String Genre = tmp[0];
            int lineNum = Integer.parseInt(tmp[1]);
            String songLyrics = tmp[2];
            Songlines[i] = new SongLine(Genre, lineNum, songLyrics);

        }
        return Songlines;
    }

    /*
    * Prints out all items in an array that have a genre equal to filterWord.
    * The first output line is: 
    * Lines by <filterWord>
    */
    public static void listLinesByGenre(SongLine[] songs,
                                    String filterWord) {
        System.out.println("Lines by " + filterWord);
        for (SongLine line : songs) {
            if (line.getGenre().equals(filterWord)){
                System.out.println(line);    
            }
        }
    }

    /*
    * Prints out the elements of an array.
    */
    public static void printArray(SongLine[] songs) {
        for(SongLine line : songs) {
            System.out.println(line);
        }
    }

    /*
    * Sorts an array of SongLine objects by their line number in ascending
    * order.
    */
    public static void sortByLineNumber(SongLine[] songs) {
        quickSort(songs, 0, songs.length-1);
    }

    public static void quickSort(SongLine [] lines, int low, int high) {
        if (low >= high) {
            return;
        }
        int pivot = lines[(low + high)/2].getLineNumber();
        int index = partition(lines, low, high, pivot);
        quickSort(lines, low, index -1);
        quickSort(lines, index, high);
    }

    public static int partition (SongLine []  lines, int left, int right, int pivot) {
        while(left <= right) {
            while(lines[left].getLineNumber() < pivot) {
                left++;
            }

            while(lines[right].getLineNumber() > pivot) {
                right--;
            }

            if (left <= right) {
                SongLine tmp = lines[right];
                lines[right] = lines[left];
                lines[left] = tmp;
                left++;
                right--;
            }
        }
        return left;
    }

    /*
    * Prints out a custom 21-line song, randomly selecting words from the
    * array of SongLine objects.
    */
    public static void makeSong(SongLine[] songs) {
        Random rand = new Random();
        for(int i = 0; i < 21; i++) {
            System.out.println(songs[rand.nextInt(songs.length)].getWords());
        }

    }

    public static Scanner getSecondScanner(Scanner console) {
        try {
            System.out.print("What is the input file name: ");
            String fileName = console.next();
            File f = new File(fileName);
            if (f.exists()) {
                Scanner file = new Scanner(f);
                return file;
            } else {
                System.out.println("That file does not exist.");
                return getSecondScanner(console);
            }
            
        } catch (FileNotFoundException e) {
            System.exit(0);
        }
        return null;
    }

    /*
    * Creates two Scanner objects: one that reads input from the console,
    * and another that scans a file. The user is promped to enter a filename
    * until the second Scanner object can link to that file. This method is
    * also used as a test harness for each of the methods.
    */
    public static void main(String[] args) {
        Scanner console = new Scanner(System.in);
        
        Scanner file = getSecondScanner(console);
        if (file.equals(null)){
            System.out.println("Something is wrong");
        }
        
        SongLine [] lines = makeArray(file); //make array test
        file.close();
        printArray(lines); //print array test
        System.out.println("End of makeArray and printArray tests");

        System.out.print("What genre are you looking for: ");
        String genre = console.next();
        System.out.println("Lines by " + genre);
        listLinesByGenre(lines, genre); //list liens test
        System.out.println("End of listLinesByGenre test");

        System.out.println("Before Sorting:");
        printArray(lines);
        sortByLineNumber(lines); //sorting test
        System.out.println("After Sorting:");
        printArray(lines);
        System.out.println("End of sortByLineNumber test");

        System.out.println("THE NEW SONG");
        makeSong(lines); //make song test
        System.out.println("End of makeSong test");
    }
}