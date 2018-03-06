import java.util.*;

public class CoinFlip implements Runnable {
    int thread_id; 
    private int counter;
    public int localHeads = 0;
    private Random rand;

    public void run() {
        for(int i = 0; i < counter; i++) {
            if (rand.nextInt(2) == 0) {
                localHeads++;
            }
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
        int totalHeads = 0;
        Thread[] allThreads = new Thread[numThreads];
        CoinFlip[] objs = new CoinFlip[numThreads];
        for (int i = 0; i < numThreads; i++) {
            if(i == 0) {
                CoinFlip coin = new CoinFlip(i, counter + extra);
                objs[i] = coin;
                allThreads[i] = new Thread(coin);
                allThreads[i].start();
            }
            else {
                CoinFlip coin = new CoinFlip(i, counter + extra);
                objs[i] = coin;
                allThreads[i] = new Thread(coin);
                allThreads[i].start();
            }

        }
        for (int j = 0; j < numThreads; j++) {
            try {
                allThreads[j].join();
                totalHeads += objs[j].localHeads;
            }
            catch (Exception e)
            {
                System.out.println("Thread interrupted.  Exception: " + e.toString() +
                           " Message: " + e.getMessage()) ;
                return;
            }

        }
        System.out.println(totalHeads + " heads in "+ Integer.parseInt(args[1])+ " coin tosses");
        end = System.currentTimeMillis() - start;
        System.out.println("Elapsed Time: "+ end + "ms");
    }
}
