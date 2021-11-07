# approxcount
A lab exercise exploring approximate (i.e., sloppy) counting.

In this lab, you'll:

 * implement a few ways of doing approximate counting (and one non-approximate way). 
 * run some experiments to gather data
 * do some performance analysis to see how the options change the behaviors.

## implementation tasks

**Hint: Write code for every example of 'FIXME' in the base program. We suggest you fix the FIXMEs in the following order (but you will have to figure out which FIXME will solve each task):**

In order, these are:
 1. Add code to join on each created pthread before gathering timing statistics. 
 1. Code for the function 'noslop'. This function uses a mutex to protect the global counter value and increments it only by 1 each round.
 1. Add a call to pthread\_create to make a sloppy thread.
 1. Code for the function 'sloppy'. This function performs sloppy counting where it counts a local value up until it reaches the slop value specified in the thread args pointer. Then it acquires a global mutex, adds it to the global counter, and releases. 'sloppy' should **not** add its result to the global total if the total has already reached the value **target**. **NOTE**: For the 'random1' mode, the slop value is a random number between 1 and (2 * slop) (where 'slop' is specified on the command line). A random slop value is only rolled once per thread. There is no function 'random1' -- that's because you can use the function 'sloppy' to perform the task for 'random1'. Random slop values can result in counts that are slightly higher than the target value.
 1. Create code using rand() to acquire a proper random number for 'random1' mode.
 1. Add a call to pthread\_create to make a new random1 thread.
 1. Add a call to pthread\_create to create a new random2 thread. (The 'random2' function should make calls to rand() after each local count loop.)
 1. Code for the function 'random2'. Unlike for 'random1', 'random2' generates a new slop value using rand() for every local count up to the slop value. Like for 'random1', the slop value should be between 1 and (2 * slop) (the -s parameter on the command line).
 
## experiments and analysis

Test that your code is working by building your program using 'build.sh' and by issuing commands, e.g.:

`./sloppycount -n 1000000000 -s 1000 -r 5 -m sloppy -t 4`

Test all the parameters to make sure that you've implemented everything.

Perform this work on ukko or akka (other machines may not have enough cores).

Once your code is working, execute the bash scripts below to gather experimental data. Cut and paste the data into a spreadsheet (one table per sheet). It should import into columns if you do it correctly. Then, calculate the average value for each column, and plot the averages as a series with respect to the changing value. Answer the questions about the data in a text file under headings for each question below.

**NOTE:** if your login shell is not `bash`, you will need to run the command `bash` before running the testing commands below. To find out what your shell is, execute:

```
$ echo $SHELL
```

### test1.sh: Sloppy Counting -- Threads vs. Runtime

Execute `test1.sh` on ukko or akka. 

#### Tasks & Questions

0. Create a line plot showing how runtime changes as more threads are added. 

1. What do you observe? 

2. How does runtime change as the number of threads increase?

### test2.sh: Sloppy Counting -- Sloppiness vs. Runtime

Execute `test2.sh` on ukko or akka.

### Tasks & Questions

3. Create a line plot showing how runtime changes as the sloppiness value increases. 

4. How does runtime change as the sloppiness value increases?

### test3.sh: Accurate Counting: Threads vs. Runtime

Execute `test3.sh` on ukko or akka.

### Tasks & Questions

5. Create a line plot showing how runtime changes with an increase in the number of threads. 

6. How does the number of threads affect runtime when there's no slop?

### test4.sh: Sloppy Counting (random1) -- Runtime vs. Threads

Execute `test4.sh` on ukko or akka.

### Tasks & Questions

7. Create a line plot showing how *average* runtime changes with an increase in the number of threads. 

8. How does runtime for random1 change as the number of threads increase?

### test5.sh: Sloppy Counting (random2) -- Runtime vs. Threads

Execute `test5.sh` on ukko or akka.

### Tasks & Questions

9. Create a line plot showing how *average* runtime changes with an increase in the number of threads. 

10. How does runtime for random2 change as the number of threads increase?

### Compare sloppy vs. noslop

Combine the results for sloppy and noslop.

### Tasks & Questions

11. Make a graph showing sloppy vs. noslop runtime.

In a paragraph, compare and contrast the methods. Make sure you explain:

12. Why do you think you're seeing the results you are? 
13. Which is fastest? Why?
14. Which is slowest? Why?
15. Which is most accurate? Why?
 
### Compare random methods

16. Now, plot separate line series of the random1 and random2 data points (from `test4.sh` and `test5.sh`). Don't plot averages; instead the y-axis should show time but the x-axis will have one data point per observation (one line per experiment). 
17. What do you observe about the smoothness of random1 vs. random2? Explain why things are the way they are.
18. Which is faster? Why?

## Example graphs

In the `img/` directory there are sample images to show what the general trends should look like. Data for the random tests may vary more widely due to randomness. If you do not see similar trends (especially for sloppy counting), check to make sure that your local counting happens in a tight loop -- in other words, your counting threads do nothing but count (e.g., they don't print things) until they've reached the slop value and only then add it to the global value.

## submission

Submit your code, your spreadsheet, and a document containing your graphs inline with your answers (either linked to the spreadsheet or screenshotted) in a gzipped tarball (i.e., `whatever.tar.gz`) via the course website by the due date.
