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
 echo "5. setup files"
 echo "6. setup allegro"
 echo "7. exit"
 echo -n "Please enter your choice? "
}
# Loop while the variable choice is equal 4
# bash while loop
menu

while [ $choice -ne 7 ]; do
 
# read user input
read choice
# bash nested if/else
if [ $choice -eq 1 ] ; then
        ifconfig
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
                        	gedit readme.txt
                        	menu
                        else
                        	if [ $choice -eq 5 ] ; then
                                chmod +x setup.sh
                        		./setup.sh
                        		menu
                        	else
                        		if [ $choice -eq 6 ] ; then
                        			chmod +x setup_allegro.sh
                                    ./setup_allegro.sh
                        			menu
                        		else 
                        			if [ $choice -eq 7 ] ; then
                                        exit
                                    else
                                        echo "Enter valid input"
                                        menu
                                    fi
                        		fi
                        	fi
                        fi
                fi   
        fi
fi
done 