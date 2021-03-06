Pyraminx.. The Untold Story
/////
You can see a video demonstrating its setup and working here :
https://www.youtube.com/watch?v=EBYpmsQXVZQ&feature=youtu.be
/////
Setting up the Utility
You will need to install Allegro in order to run this utility. To install Allegro, you can run the file setup_allegro.sh
Navigate to this directory in the Terminal and type "chmod +x setup_allegro.sh" to make it an executable file.
Run the setup file by typing "./setup_allegro.sh"
Alternatively, you could follow a detailed guide here : https://wiki.allegro.cc/index.php?title=Install_Allegro5_From_Git/Linux/Debian
Once setup, run ./setup.sh to compile all files. It should be made an executable as well by running "chmod +x setup.sh"

Alternatively, you could fire ./menu.sh and select options 6 and 5 in that order. Make sure menu.sh is an executable. Run "chmod +x menu.sh" to do the same.

Running the standard modes :

Simply type "./menu.sh" (It should be an executable, done with the step mentioned above.) A menu bar pops up. Here we can select the mode we wish to use. Further operation details are mentioned on the screen. Make sure you close Allegro Windows after you are done. While solutions are being computed, please be patient and wait upto 15-20 seconds. On an average, it would take 4-6 seconds. For some cases, or unsolvable states, it would take upto 15-20 seconds.

Mode 1 :- This mode runs via the Android app provided along with this utility. Make sure that the Android app and the PC have been connected via the same WiFi router or via WiFi Hotspot. Use the IP address mentioned in the Android app and select port 6066. While clicking photographs, make sure the Pyraminx face points upwards. Press Send after clicking the picture. Make sure all stickers have been detected. Detected stickers are surrounded by black lines. After clicking the first picture, go to the face on the RIGHT of the start face. After this face, turn the cube again, to see the face LEFT of the start face.
For the last face, the bottom right corner with respect to the start face should point upwards.
The rest of the steps have been automated.
For best results, try not to let light bounce off the stickers. They are covered with a glossy plastic film. Also use a black based Pyraminx. Also the stickers should not be too dark in the image.

Mode 2 :- Use the interface to attempt solving the Pyraminx. You may also use the shuffling algorithm mentioned and solve it on your Pyraminx. Make sure you record your solution on the GUI. Close the window to see a detailed analysis on your solution.

Mode 3 :- Use the GUI to find the start state of the Pyraminx. White stickers imply that this sticker could be any colour. Press done to generate your algorithm. The algorithm will convert this cube to a solved cube. Let's say we wish to see an algorithm to exchange all three face pieces clockwise. So your input would have the face pieces rotated in anticlockwise direction. Please enter a valid solvable Pyraminx here. This is essential to generate an algorithm, else no solution will be found.

Mode 4 :- View this file.

Mode 5 :- Compile all files.

Mode 6 :- Setup up Allegro directly.

Mode 7 :- Exit

Executable and Text files and how to use them :-
Symbols used in this project :-
0,1,2,3,4 represent Blue, Red, Green, Yellow and White in sticker colour files like test.txt
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 represent U, U', L, L', R, R', B, B', u, u', l, l', r, r', b, b' moves with respect to start face as in files Analysis.txt, solved_moves.txt

algorithm_finder : This file reads test.txt and generates the Pyraminx algorithm based on the data in this file. It prints it in the terminal and writes it to file solved_moves.txt

algorith_finder_gui : This file is used to input the algorithm finder input using an Allegro GUI. It writes the input to test.txt using the code mentioned above.

analysis : This file reads Analysis.txt and produces an analysis for this shuffle and attempt. It prints the analysis to the terminal. It provides the shortest solution after each move.

Analysis.txt : The first line represents the shuffle move symbols using the numbers mentioned above. The second line is the user's attempt.

bugs.txt : List of bugs reported with their status.

camera.apk : The android app.

camera : The Android Studio Project for our app.

GreetingServer.class : The Java server which will receive input and write it to test.txt

image_processed_cube_solver : A terminal utility which reads test.txt and generates the solution. It writes the solution to solved_moves.txt

menu.sh : The bash script which opens the menu.

pyraminx_shuffler : An Allegro GUI which is used to shuffle the pyraminx, and allow the user to solve the pyraminx. It writes the shuffle sequence and user's attempt to Analysis.txt

pyraminix_solver : An old executable not used by menu.sh. It simply reads test.txt and prints the shortest solution.

readme.txt : THIS FILE

setup_allegro.sh : A bash script to automatically install allegro

setup.sh : A bash script to automatically compile all files and magically lubricate the Pyraminx.

shuffler : An old executable which isn't used. Old version of pyraminx_shuffler

solution_display : An Allegro GUI which reads solved_moves.txt and displays the solution using arrows.

solved_moves.txt : A shortest solution, written by various files and read by solution_display

TEST FILES : All our experiments :D

test.txt : Holds sticker colours. One face per line, nine numbers for nine stickers

WORK DIVISION CS101.txt : A memorable and fun journey..

Developed by :-
Kalpesh Krishna
Karan Chadha
Roshan Nayak
Harita Parmar

IIT Bombay....ofcourse. B)