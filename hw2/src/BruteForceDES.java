import java.io.File;
import java.io.IOException;
import java.io.PrintStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Random;

import javax.crypto.SealedObject;

public class BruteForceDES implements Runnable{
    public BruteForceDES() {
        
    }
    @Override
    public void run() {
        // TODO Auto-generated method stub
        
    }
    public static void main(String[] args) throws IOException {
        if(args.length != 3) {
            System.out.println("Usage: java BruteForceDES #threads key_size_in_bits <filename>");
            return;
        }
        // create object to printf to the console
        PrintStream p = new PrintStream(System.out);
        int threads = Integer.parseInt(args[0]);
        // Get the argument
        long keybits = Long.parseLong ( args[1] );

        long maxkey = ~(0L);
        maxkey = maxkey >>> (64 - keybits);

        // Create a simple cipher
        SealedDES enccipher = new SealedDES ();

        // Get a number between 0 and 2^64 - 1
        Random generator = new Random ();
        long key =  generator.nextLong();

        // Mask off the high bits so we get a short key
        key = key & maxkey;

        // Set up a key
        enccipher.setKey ( key );

        // Get the filename
        String filename = args[2];
        // Read in the file to encrypt
        File inputFile = new File(filename);

        // Turn it into a string
        if (!inputFile.exists()) {
            System.err.println("error: Input file not found.");
            System.exit(1);
        }
        byte[] encoded = Files.readAllBytes(Paths.get(filename));

        String plainstr = new String(encoded, StandardCharsets.US_ASCII);

        // Encrypt
        SealedObject sldObj = enccipher.encrypt ( plainstr );

        // Here ends the set-up.  Pretending like we know nothing except sldObj,
        // discover what key was used to encrypt the message.

        // Get and store the current time -- for timing
        long runstart;
        runstart = System.currentTimeMillis();

        // Create a simple cipher
        SealedDES deccipher = new SealedDES ();

        // Search for the right key
        for ( long i = 0; i < maxkey; i++ )
        {
            // Set the key and decipher the object
            deccipher.setKey ( i );
            String decryptstr = deccipher.decrypt ( sldObj );

            // Does the object contain the known plaintext
            if (( decryptstr != null ) && ( decryptstr.contains(plainstr)))
            {
                //  Remote printlns if running for time.
                //p.printf("Found decrypt key %016x producing message: %s\n", i , decryptstr);
                System.out.println (  "Found decrypt key " + i + " producing message: " + decryptstr );
            }

            // Update progress every once in awhile.
            //  Remote printlns if running for time.
            if ( i % 100000 == 0 )
            {
                long elapsed = System.currentTimeMillis() - runstart;
                System.out.println ( "Searched key number " + i + " at " + elapsed + " milliseconds.");
            }
        }

        // Output search time
        long elapsed = System.currentTimeMillis() - runstart;
        long keys = maxkey + 1;
        System.out.println ( "Completed search of " + keys + " keys at " + elapsed + " milliseconds.");
    }
    
}
