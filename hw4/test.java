//William Ye (wye11), Jason Zhang (jzhan127)
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Scanner;


/**
  * Cache simulator.
  */
public final class CacheSimulator2 {
    public static int loads;
    public static int stores;
    public static int loadhits;
    public static int loadmisses;
    public static int storehits;
    public static int storemisses;
    public static int cycles;
    public static int[] userInp = new int[6];
    public static fields[][] stored;

    /**
      * Private constructor to make checkstyle happy.
      */
    private CacheSimulator2(){

    }

    //helper method to make code cleaner, if writeallocate or not
    private static boolean writeAllocate(int[] inp) {
        if (inp[3] == 1) {
            return true;
        }

        return false;
    }

    private static boolean writeThrough(int[] inp) {
        if (inp[4] == 1) {
            return true;
        }

        return false;
    }

    private static boolean lru(int[] inp) {
        if (inp[5] == 1) {
            return true;
        }

        return false;
    }

    private static boolean hitCheck(fields[][] stored, fields n) {
        boolean hit = false;
        for (int i = 0; i < stored[Integer.parseInt(n.getIndex(), 2)].length; i++) {
            if (stored[Integer.parseInt(n.getIndex(), 2)][i] != null) {
                if (stored[Integer.parseInt(n.getIndex(), 2)][i].getTag().equals(n.getTag())) {
                    hit = true;
                    break;
                }
            }
        }
        return hit;
    }

    private static int hitFound(fields[][] stored, fields n) {
        int found = 0;
        for (int i = 0; i < stored[Integer.parseInt(n.getIndex(), 2)].length; i++) {
            if (stored[Integer.parseInt(n.getIndex(), 2)][i] != null) {
                if (stored[Integer.parseInt(n.getIndex(), 2)][i].getTag().equals(n.getTag())) {
                    found = i;
                    break;
                }
            }
        }
        return found;
    }

    public static void main(String[] args) throws IOException {
        for (int i = 0; i < args.length - 1; i++) {
            userInp[i] = Integer.parseInt(args[i]);
        }
        stored = new fields[userInp[0]][];

        try {
            Scanner inFile = new Scanner(new FileReader(args[args.length - 1]));   
        } catch (IOException e) {
            System.err.println("IO EXCEPTION.");
            System.exit(-1);
        }

        try {
            Scanner inFile = new Scanner(new FileReader(args[args.length - 1]));

        } catch (FileNotFoundException e) {
            System.err.println("FILE NOT FOUND.");
            System.exit(-1);
        }

        Scanner inFile = new Scanner(new FileReader(args[args.length - 1]));
        String hold = "";
        while (inFile.hasNextLine()) {
            hold = inFile.nextLine();
            process(hold);
        }

        System.out.println("Total loads: " + loads);
        System.out.println("Total stores: " + stores);
        System.out.println("Load hits: " + loadhits);
        System.out.println("Load misses: " + loadmisses);
        System.out.println("Store hits: " + storehits);
        System.out.println("Store misses: " + storemisses);
        System.out.println("Total cycles: " + cycles);  

    }
    public static void process(String inp) {
        Scanner sc = new Scanner(inp);
        String op = "";
        String hex = "";
        for (int i = 0; i < 2; i++) {
            if (i == 0) {
                op = sc.next();
            }
            if (i == 1) {
                hex = sc.next();
            }
        }

        long dec = Long.decode(hex);
        String binary = Long.toBinaryString(dec);
        String bin = "";
        bin = String.format("%32s", binary).replace(" ", "0");
        int offsetNeed = (int) (Math.log(userInp[2]) / Math.log(2));
        int indexNeed = (int) (Math.log(userInp[0]) / Math.log(2));

        String tag = bin.substring(0, bin.length() - (offsetNeed + indexNeed));
        String index = bin.substring(bin.length() - (offsetNeed + indexNeed),
                (bin.length() - (offsetNeed + indexNeed)) + indexNeed);
        String offset = bin.substring((bin.length() - (offsetNeed + indexNeed)) + indexNeed, bin.length());
        fields n = new fields(tag, index, offset);

        boolean hit = false;
        int found =0;

        if (op.equals("l")) {
            if (stored[Integer.parseInt(n.getIndex(), 2)] == null) { // miss on null
                fields[] temp = new fields[userInp[1]];
                temp[0] = n;
                stored[Integer.parseInt(n.getIndex(), 2)] = temp;
                cycles++;
                loadmisses++;
                // n.setBit();
                cycles += (100 * (userInp[2] / 4) + 1);
            } else {
                hit = hitCheck(stored, n);
                found = hitFound(stored, n);
                int found2 = found;
                if (hit) { // cache hit
                    if (lru(userInp)) {
                        fields[] temp = stored[Integer.parseInt(n.getIndex(), 2)];
                        for (int i = 0; i < found; i++) {
                            fields temp2 = temp[found2];
                            temp[found2] = temp[found2 - 1];
                            temp[found2 - 1] = temp2;
                            found2--;
                        }
                        stored[Integer.parseInt(n.getIndex(), 2)] = temp;
                    }
                    loadhits++;
                    cycles++;
                } else {
                    loadmisses++; // miss not found
                    if (countElements(stored[Integer.parseInt(n.getIndex(), 2)]) == userInp[1]) { // if
                                                                                                  // full
                        fields evict = null;
                        if (lru(userInp)) {
                            evict = stored[Integer.parseInt(n.getIndex(), 2)][userInp[1] - 1];
                            stored[Integer.parseInt(n.getIndex(), 2)][userInp[1] - 1] = n;
                        } else {
                            evict = stored[Integer.parseInt(n.getIndex(), 2)][0];
                            stored[Integer.parseInt(n.getIndex(), 2)][0] = n;
                            shiftToBack(Integer.parseInt(n.getIndex(), 2));
                        }
                        if (!writeThrough(userInp) && evict.getBit()) {
                            cycles += (100 * (userInp[2] / 4)) + 1; // evict
                        }
                        // load
                    } else {
                        for (int i = 0; i < userInp[1]; i++) { // miss not fill
                            if (stored[Integer.parseInt(n.getIndex(), 2)][i] == null) {
                                stored[Integer.parseInt(n.getIndex(), 2)][i] = n;
                                break;
                            }
                        }
                    }
                    if (lru(userInp)) {
                        shiftToFront(Integer.parseInt(n.getIndex(), 2));
                    }

                    cycles += (100 * (userInp[2] / 4) + 1); // load
                    // n.setBit();
                }
            }
            loads++;

        }
        if (op.equals("s")) {
            if (writeAllocate(userInp)) {
                if (stored[Integer.parseInt(n.getIndex(), 2)] == null) { // null
                                                                         // miss
                    fields[] temp = new fields[userInp[1]];
                    temp[0] = n;
                    stored[Integer.parseInt(n.getIndex(), 2)] = temp;
                    storemisses++;
                    cycles += (100 * (userInp[2] / 4) + 1);
                    cycles++;

                    n.setBit();

                } else {
                    hit = hitCheck(stored, n);
                    found = hitFound(stored, n);
                    for (int i = 0; i < stored[Integer.parseInt(n.getIndex(), 2)].length; i++) {
                        if (stored[Integer.parseInt(n.getIndex(), 2)][i] != null) {
                            if (stored[Integer.parseInt(n.getIndex(), 2)][i].getTag().equals(n.getTag())) {
                                hit = true;
                                found = i;
                                break;
                            }
                        }
                    }
                    int found2 = found;
                    if (hit) { // hit
                        if (lru(userInp)) {
                            fields[] temp = stored[Integer.parseInt(n.getIndex(), 2)];
                            for (int i = 0; i < found; i++) {
                                fields temp2 = temp[found2];
                                temp[found2] = temp[found2 - 1];
                                temp[found2 - 1] = temp2;
                                found2--;
                            }
                            stored[Integer.parseInt(n.getIndex(), 2)] = temp;
                        }
                        if(writeThrough(userInp)){
                            cycles += 100 * (userInp[2] / 4);
                        }
                        n.setBit();
                        cycles++;
                        storehits++;
                    } else {
                        storemisses++;
                        if (countElements(stored[Integer.parseInt(n.getIndex(), 2)]) == userInp[1]) { // miss
                                                                                                      // full
                            fields evict = null;
                            if (lru(userInp)) {
                                evict = stored[Integer.parseInt(n.getIndex(), 2)][userInp[1] - 1];
                                stored[Integer.parseInt(n.getIndex(), 2)][userInp[1] - 1] = n;

                            } else {
                                evict = stored[Integer.parseInt(n.getIndex(), 2)][0];
                                stored[Integer.parseInt(n.getIndex(), 2)][0] = n;
                                shiftToBack(Integer.parseInt(n.getIndex(), 2));
                            }
                            if (!writeThrough(userInp) && evict.getBit()) {
                                cycles += ((100 * (userInp[2] / 4)) + 1);
                            }
                            //to delete?
                        } else {
                            for (int i = 0; i < userInp[1]; i++) {
                                if (stored[Integer.parseInt(n.getIndex(), 2)][i] == null) {
                                    stored[Integer.parseInt(n.getIndex(), 2)][i] = n;
                                    break;
                                }
                            }
                        }

                        if (lru(userInp)) {
                            shiftToFront(Integer.parseInt(n.getIndex(), 2));
                        }
                        cycles += ((100 * (userInp[2] / 4)) + 1);
                        cycles++;

                        n.setBit();

                    }

                }
            } else {
                if (stored[Integer.parseInt(n.getIndex(), 2)] == null) { // null
                                                                         // miss
                    cycles += 100 * (userInp[2] / 4);

                } else {
                    hit = hitCheck(stored, n);
                    if (hit) {
                        cycles += 100 * (userInp[2] / 4) + 1;
                        storehits++;
                    } else {
                        cycles += 100 * (userInp[2] / 4);
                        storemisses++;
                    }
                }
            }

            stores++;
        }
    }

    public static void shiftToFront(int a) {
        fields[] temp = stored[a];
        int s = 0;
        for (int i = 0; i < userInp[1]; i++) {
            if (temp[i] == null) {

                s = i - 1;
                break;
            }
            if (i == userInp[1] - 1) {
                s = i;

            }

        }
        int t = s;
        fields hold = temp[0];
        for (int i = 0; i < s; i++) {
            fields q = temp[t];
            temp[t] = temp[t - 1];
            temp[t - 1] = q;
            t--;
        }
        stored[a] = temp;
    }

    public static void shiftToBack(int a) {
        fields[] temp = stored[a];
        int q = countElements(stored[a]);
        for (int i = 0; i < q - 1; i++) {
            fields hold = temp[i];
            temp[i] = temp[i + 1];
            temp[i + 1] = hold;
        }
        stored[a] = temp;
    }

    public static int countElements(fields[] a) {
        int count = 0;

        for (int i = 0; i < a.length; i++) {
            if (a[i] != null) {
                count++;
            }
        }
        return count;
    }

}