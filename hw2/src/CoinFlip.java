import java.util.*;
import java.util.concurrent.atomic.AtomicInteger;

public class CoinFlip implements Runnable {
    int thread_id; 
    public static int heads = 0;
    public static int tails = 0;
    public static int counter;
    //public static int runs;
    private int localHeads = 0;
    private Random rand;

    public void run() {
        for(int i = 0; i < counter; i++) {
            if (rand.nextInt(2) == 0) {
                localHeads++;
            }    
        }
        synchronized (CoinFlip.class) {
            heads += localHeads;
        }
    }

    // Constructor: set thread id
    CoinFlip(int id) {
        this.rand = new Random();
        this.thread_id = id;
    }

    public static void main(String[] args) throws InterruptedException {
        if (args.length != 2) {
            System.out.println("Usage: CoinFlip #threads #iterations");
            System.exit(0);
        }
        long start = System.currentTimeMillis();
        long end;
        int numThreads = Integer.parseInt(args[0]);
        counter = Integer.parseInt(args[1])/numThreads;
        Thread[] allThreads = new Thread[numThreads];
        for (int i = 0; i < numThreads; i++) {
            allThreads[i] = new Thread(new CoinFlip(i));
            allThreads[i].start();

        }
        for (int j = 0; j < numThreads; j++) {
            allThreads[j].join();

        }
        System.out.println(heads + " heads in "+ Integer.parseInt(args[1])+ " coin tosses");
        end = System.currentTimeMillis() - start;
        System.out.println("Elapsed Time: "+ end + "ms");
    }
}
