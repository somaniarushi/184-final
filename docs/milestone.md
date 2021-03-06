# CS184 Final Project Milestone Check-In
[LINK TO VIDEO HERE](https://drive.google.com/file/d/1I8Fc-O18mgHnC_URKBYsCWaDiJvlGbCw/view?usp=sharing)

https://www.dropbox.com/s/cd9yd9b5v4gb09a/tubeman.mov?dl=0

[](https://drive.google.com/file/d/1I8Fc-O18mgHnC_URKBYsCWaDiJvlGbCw/view?usp=sharing)## Basic Information
- **Title****:** Mr. Inflatable Tube Man
- **Summary****:** For our project, we’ll be working on creating a realistic 3D rendering of an inflatable tube man. To do so, we will simulate particle physics and wind dynamics through a rubber tube.   
- **Team** **members****:** Ashley Chu (3034858776), Manaal Siddiqui (3034654585), Crystal Wang (3034709952), Arushi Somani (3034899544)

**The Goal**

![Inflatable Tube Man Example (he/him)](https://media2.giphy.com/media/3oriOfe0A84SVqwr6w/giphy.gif)

## Accomplishments
1. Wind Particle Simulation
2. Wind Cloth Interaction
3. Tube Construction

**Wind Simulation**
We create a wind algorithm that generates wind by defining it as a force that will act on the cloth. Here’s a particularly important chunk of our changes:


      float consC = 0.5; //represents strength of wind
      Vector3D windDirV = Vector3D(1, 1, 1);
      Vector3D inside = dot(normal, windDirV - v) * normal;
      Vector3D windForce = consC * inside;

This algorithm is deceptively simple. We look forward to adding more aspects to our wind algorithm, refactoring it, or making it more complex. We will increase the complexity of our wind simulation algorithm as required for the project — as we’re trying to not spend too much time on developing particle simulation elements that can be fabricated more easily otherwise.

**Wind Cloth Interaction**


![Wind interacting with cloth](https://paper-attachments.dropbox.com/s_BE77F21AE4081DC715A1FAA72AC964320DCBFA5B9B57EBD7836954DF7AB1C06E_1651018941698_windtest.gif)


By integrating the above algorithm into the cloth simulation `simulate` function, we were able to create acceleration on the cloth through our wind simulation algorithm.

**Tube Construction**

![Beginning Attempts!](https://paper-attachments.dropbox.com/s_BE77F21AE4081DC715A1FAA72AC964320DCBFA5B9B57EBD7836954DF7AB1C06E_1651025135554_image.png)


To begin our tube construction, we built off of our Project 4 Cloth Simulation project in terms of structuring our object with springs and point masses. Instead of approaching our mesh as a cloth, we had to think of springs and point masses as thread forming our tube. We primarily edited our `build_grid()` in `cloth.cpp` .

Seeing as how our mesh generation functions use a row-column format to represent the structure, the easiest stepping point was to create a rigid mesh using the concept of combining 4 individual meshes as sides which were then combined.


![Rectangular tube mesh](https://paper-attachments.dropbox.com/s_BE77F21AE4081DC715A1FAA72AC964320DCBFA5B9B57EBD7836954DF7AB1C06E_1651025171745_image.png)
![Wind flowing through rectangular tube](https://paper-attachments.dropbox.com/s_BE77F21AE4081DC715A1FAA72AC964320DCBFA5B9B57EBD7836954DF7AB1C06E_1651025370129_Apr-26-2022+19-07-10.gif)





![](https://paper-attachments.dropbox.com/s_BE77F21AE4081DC715A1FAA72AC964320DCBFA5B9B57EBD7836954DF7AB1C06E_1651020081954_Screen+Shot+2022-04-26+at+5.41.13+PM.png)
![Wind flowing through open cloth tube](https://paper-attachments.dropbox.com/s_BE77F21AE4081DC715A1FAA72AC964320DCBFA5B9B57EBD7836954DF7AB1C06E_1651020007547_Apr-26-2022+17-39-43.gif)
![](https://paper-attachments.dropbox.com/s_BE77F21AE4081DC715A1FAA72AC964320DCBFA5B9B57EBD7836954DF7AB1C06E_1651020051078_Screen+Shot+2022-04-26+at+5.38.17+PM.png)


However, we then realized we could use polar coordinates to represent the tube shape. With the help of polar coordinates, we were able to construct a cylindrical shape for our tube.


![](https://paper-attachments.dropbox.com/s_BE77F21AE4081DC715A1FAA72AC964320DCBFA5B9B57EBD7836954DF7AB1C06E_1651020054719_Screen+Shot+2022-04-26+at+5.37.35+PM.png)


We edited `json` files to be able to create a tube, by taking inspiration from the sphere.json file!

## Preliminary Results and Reflections on Progress

Tube man is possible! We’re on track!

For final touches, we plan to implement a UV mapped version of the tube by using and illustrator design textured onto our tube.


## Updates to Work Plan

Maybe can’t do tube man features like face or arms, 3D modeling is difficult.

One of the more difficult parts of this project has been constructing our tube. Specifically, because the object we ideally want to work with is complex/abstract, adding arms as an additional component currently seems like a difficult feat. Additionally, adding an enclosure for the tube seems a bit difficult, along with representing the internal and external forces of the tube.

This week, we went to office hours to check-in on our progress with TAs who gave us guidance to look into the triangulation algorithm and OpenGL resources to construct our tube mesh.



## References + Resources

[Original Project Proposal Doc](https://somaniarushi.github.io/184-final/)
[](http://)[TA Feedback](https://docs.google.com/document/d/1ixCnejDpCs4ehfYdg3bs965VQ3QEcITUs65Pfs1OX9M/edit)
[Repository](https://github.com/somaniarushi/184-final.git)
[Link to Video Slides](https://docs.google.com/presentation/d/1AVXhw1rygKwHzyE5RwmPW3Jedo3_5Xamv6ygRoUikg4/edit?usp=sharing)
