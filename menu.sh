#!/bin/bash
 
# Declare variable choice and assign value 4
choice=0
# Print to stdout
function menu
{
 echo "Please choose the standard utility you desire."
 echo "1. receive data from app and find shortest solution"
 echo "2. shuffle and solve a pyraminx and get a complete analysis"
 echo "3. generate algorithms for your need"
 echo "4. get help"
 echo "5. exit"
 echo -n "Please enter your choice? "
}
# Loop while the variable choice is equal 4
# bash while loop
menu

while [ $choice -ne 5 ]; do
 
# read user input
read choice
# bash nested if/else
if [ $choice -eq 1 ] ; then
 
        java GreetingServer 6066
        ./image_processed_cube_solver
        ./solution_display
        menu

else                   

        if [ $choice -eq 2 ] ; then
                 ./pyraminx_shuffler
                 ./analysis
                 menu
        else
         
                if [ $choice -eq 3 ] ; then
                        ./algorithm_finder_gui
                        ./algorithm_finder
                        ./solution_display
                        menu
                else
                        if [ $choice -eq 4 ] ; then
                        	subl readme.txt
                        	menu
                        else
                        	echo "Enter something valid"
                        fi
                fi   
        fi
fi
done 