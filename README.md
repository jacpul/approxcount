# sloppycount
A lab exercise exploring sloppy counting.

In this lab, you'll:

 * implement a few ways of doing sloppy counting (and one non-sloppy way). 
 * run some experiments to gather data
 * do some performance analysis to see how the options change the behaviors.

## implementation tasks

**Hint: Write code for every example of 'FIXME' in the base program.**

In order, these are:

 1. Code for the function 'sloppy'. This function performs sloppy counting where it counts a local value up until it reaches the slop value specified in the thread args pointer. Then it acquires a global mutex, adds it to the global counter, and releases. 'sloppy' should **not** add its result to the global total if the total has already reached the value **target**. **NOTE**: For the 'random1' mode, the slop value is a random number between 1 and (2 * slop) (where 'slop' is specified on the command line). A random slop value is only rolled once per thread. There is no function 'random1' -- that's because you can use the function 'sloppy' to perform the task for 'random1'. Random slop values can result in counts that are slightly higher than the target value.
 1. Code for the function 'random2'. Unlike for 'random1', 'random2' generates a new slop value using rand() for every local count up to the slop value. Like for 'random1', the slop value should be between 1 and (2 * slop) (the -s parameter on the command line).
 1. Code for the function 'noslop'. This function uses a mutex to protect the global counter value and increments it only by 1 each round.
 1. Add a call to pthread\_create to make a sloppy thread.
 1. Create code using rand() to acquire a proper random number for 'random1' mode.
 1. Add a call to pthread\_create to make a new random1 thread.
 1. Add a call to pthread\_create to create a new random2 thread. (The 'random2' function should make calls to rand() after each local count loop.)
 1. Add code to join on each created pthread before gathering timing statistics. 

## experiments and analysis

Test that your code is working by building your program using 'build.sh' and by issuing commands, e.g.:

`./sloppycount -n 1000000000 -s 1000 -r 5 -m sloppy -t 4`

Test all the parameters to make sure that you've implemented everything.

Perform this work on ukko or akka (other machines may not have enough cores).

Once your code is working, execute the bash one-liners below to gather experimental data. Cut and paste the data into a spreadsheet (one table per sheet). It should import into columns if you do it correctly. Then, calculate the average value for each column, and plot the averages as a series with respect to the changing value. Answer the questions about the data in a text file under headings for each question below.

### How does the number of threads affect the time to perform sloppy counting?

Execute the following command (Note when executing this command on ukko or akka first type `bash`):

`rm data*.txt; for threads in 1 2 4 8 16; do ./sloppycount -n 1000000000 -s 1000 -r 10 -m sloppy -t $threads > data${threads}.txt; done; paste data1.txt data2.txt data4.txt data8.txt data16.txt`

Create a line plot showing how runtime changes as more threads are added. What do you observe? How does runtime change?

### How does the sloppiness value change the runtime?

Execute the following command:

`rm data*.txt; for slop in 100 200 300 400 500 600; do ./sloppycount -n 1000000000 -s $slop -r 10 -m sloppy -t 4 > data${slop}.txt; done; paste data100.txt data200.txt data300.txt data400.txt data500.txt data600.txt`

Create a line plot showing how runtime changes as the sloppiness value increases. How does runtime change as the sloppiness value increases?

### How does the number of threads affect runtime with no slop?

Execute the following command:

`rm data*.txt; for threads in 1 2 4 8; do ./sloppycount -n 10000000 -s 1 -r 10 -m noslop -t $threads > data${slop}.txt; done; paste data1.txt data2txt data4.txt data8.txt`

Create a line plot showing how runtime changes with an increase in the number of threads. How does the number of threads affect runtime when there's no slop?

### How does random1 fare?

Execute the following command:

`rm data*.txt; for threads in 1 2 4 8 16; do ./sloppycount -n 1000000000 -s 1000 -r 10 -m random1 -t $threads > data${threads}.txt; done; paste data1.txt data2.txt data4.txt data8.txt data16.txt`

Create a line plot showing how runtime changes with an increase in the number of threads. How does random1 change as the number of threads increase?

### How does random2 fare?

Execute the following command:

`rm data*.txt; for threads in 1 2 4 8 16; do ./sloppycount -n 1000000000 -s 1000 -r 10 -m random2 -t $threads > data${threads}.txt; done; paste data1.txt data2.txt data4.txt data8.txt data16.txt`

Create a line plot showing how runtime changes with an increase in the number of threads. How does runtime change as the number of threads increase?

### Compare sloppy methods

Combine the results for sloppy, random1 and random2. In a paragraph, compare and contrast. Why do you think you're seeing the results you are? Which is fastest? Which is slowest? Why?

### Compare random methods

Now, plot separate line series of the random1 and random2 data points. Don't plot averages; instead the y-axis should show time but the x-axis will have one data point per observation. What do you observe about the smoothness of random1 vs. random2? Which is faster? Why do you think that is?

## submission

Submit your code, your spreadsheet, and your text answers in a tarball via Moodle by the due date.
