# SeeK the Game

NOTE: The newest commits come after the assignment deadline. For grading, please look at the commit "harder enemies".

Author: Sirui Huang (siruih)

Design: People tend to omit the dangers around them... Particularly in this game. You control an cartoon eyeball to dodge spike balls and reach goals, although you surroundings will gradually get masked. Predict where to dodge!

Screen Shot:

![Screen Shot](screenshot.png)

How Your Asset Pipeline Works:
The asset pipeline read png images from ~/dist/assets and read them as patterns to draw with the ppu466. The filenames are read to determine which type of assets the files belong to. Below is the avatar you'll control!

![Eyeball](/dist/assets/eye_idle.png)
![LargeEyeball](/dist/post/EyePic.png)

How To Play:
Control the eyeball with arrow keys and avoid the spike balls. Look for this:

![Goal](/dist/assets/goal.png)
![LargeGoal](/dist/post/GoalPic.png)

Contacting it will ramp up the difficulty. With higher difficulty, your avatar will be masked with more shadowy region and spike balls will move faster. 

This game was built with [NEST](NEST.md).

