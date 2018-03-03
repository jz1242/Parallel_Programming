import java.util.*;

public class CoinFlip implements Runnable {
    int thread_id; 
    public static int heads = 0;
    public static int tails = 0;
    private int counter;
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
    CoinFlip(int id, int count) {
        this.rand = new Random();
        this.thread_id = id;
        this.counter = count;
    }

    public static void main(String[] args) throws InterruptedException {
        if (args.length != 2) {
            System.out.println("Usage: CoinFlip #threads #iterations");
            System.exit(0);
        }
        long start = System.currentTimeMillis();
        long end;
        int numThreads = Integer.parseInt(args[0]);
        int counter = Integer.parseInt(args[1])/numThreads;
        int extra = Integer.parseInt(args[1]) % numThreads;
        Thread[] allThreads = new Thread[numThreads];
        for (int i = 0; i < numThreads; i++) {
            if(i == 0) {
                allThreads[i] = new Thread(new CoinFlip(i, counter + extra));
                allThreads[i].start();
            }
            else {
                allThreads[i] = new Thread(new CoinFlip(i, counter));
                allThreads[i].start();
            }

        }
        for (int j = 0; j < numThreads; j++) {
            try {
                allThreads[j].join();
            }
            catch (Exception e)
            {
                System.out.println("Thread interrupted.  Exception: " + e.toString() +
                           " Message: " + e.getMessage()) ;
                return;
            }

        }
        System.out.println(heads + " heads in "+ Integer.parseInt(args[1])+ " coin tosses");
        end = System.currentTimeMillis() - start;
        System.out.println("Elapsed Time: "+ end + "ms");
    }
}
