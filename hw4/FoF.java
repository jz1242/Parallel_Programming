
import java.io.IOException;
import java.util.*;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;


public class FoF {
    public static class myMapper extends Mapper<Object, Text, Text, Text>{
        private Text word = new Text();
        public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
            String[] people = value.toString().split("\\s+");
            for (int i = 1; i < people.length; i++) {
                for (int j = 1; j < people.length; j++) {
                    if(i != j){
                        if(Integer.parseInt(people[0]) < Integer.parseInt(people[j])){
                            word.set(people[i] + " " + people[0] + " " + people[j]);
                            context.write(word, new Text("one"));
                        }
                        else{
                            word.set(people[i] + " " + people[j] + " " + people[0]);
                            context.write(word, new Text("one"));
                        }

                    }

                }
            }
        }
    }
    
    public static class myReducer extends Reducer<Text,Text,Text,Text> {
        private Text word = new Text();
        public void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {
            int sum = 0;
            for (Text value : values) {
                sum++;
                
            }
            if(sum == 2){
                context.write(key, null);
            }
            
        }
    }

    public static void main(String[] args) throws Exception {
        Configuration conf = new Configuration();
        Job job = Job.getInstance(conf, "fof");
        job.setJarByClass(FoF.class);
        job.setMapperClass(myMapper.class);
        job.setReducerClass(myReducer.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(Text.class);
        FileInputFormat.addInputPath(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path(args[1]));
        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
}