# CS184 Final Project Proposal

## Basic Information
- **Title****:** Mr. Inflatable Tube Man
- **Summary****:** For our project, we’ll be working on creating a realistic 3D rendering of an inflatable tube man. To do so, we will simulate particle physics and wind dynamics through a rubber tube.   
- **Team** **members****:** Ashley Chu, Manaal Siddiqui, Crystal Wang, Arushi Somani


## Problem Description
- **Context:** 
![Inflatable Tube Man Example](https://media2.giphy.com/media/3oriOfe0A84SVqwr6w/giphy.gif)

- **Problem We’re Trying to Solve**: In researching this idea, we noticed that the inflatable tube man posed an interesting challenge in combining particle physics and material modelling. We have yet to find anything that contributes to such a field, and we’d like to test the interplay between the two in our project. 
- **Where it is Challenging:** 
    - First, we must define the particle physics for wind dynamics, which we haven’t yet learned in the context of graphics.
    - Second, we must define the tube man’s body, which requires deformation physics, as well as self-collision physics.
    - Third, we must define the interaction between particles and our tube man body, to be able to simulate the tube man’s characteristic motion.
- **Our Methods to a Solution:** 
    - We’ll first create a particle physics engine which we can test with some basic renders
    - Afterwards, we can build on project 4 to render a 3D tube with spring-like dynamics
    - Finally, we’ll work on simulating motion by combining the two functionalities above, defining real-time rendering using our particle physics engine.
    
## Goals and Deliverables
- This is the most important part of your proposal. You should carefully think through what you are trying to accomplish, what results you are going for, and why you think you can accomplish those goals. 


- **What we’re trying to accomplish:**
    - A verisimilitudinous 3D rendering of an inflatable tube man.
- **What results we’re going for:**
    - Accurate particle simulations through a cloth-like tube. If we design our cloth rendering and particle motion correctly, the tube man’s motion should follow naturally. This is the result we’re hoping to accomplish.
- **Why we think we can accomplish these goals:**
    - We can reuse some of the logic from project 4 for material rendering of the tube man himself, including the logic for self-collisions and lighting at different points of time 
    - We have also found libraries online that assist in particle simulations, hopefully helping us bootstrap this idea faster.


- **Answering questions mentioned:**
****    - Since this is a graphics class you will likely define the kind of images you will create (e.g. including a photo of a new lighting effect you will simulate).
![Air Powered Dancers & Where to Use Them | FFN Blog](https://s6g6d3j3.rocketcdn.me/blog/wp-content/uploads/income-tax-tube-man.gif)

    - Video of example Tube Man: https://youtube.com/clip/UgkxyJOGWmChZJiZH6XQHvELhqTqA0Tc8kv2
    - If you are working on an interactive system, describe what demo you will create.
        - Our system is not interactive to given with. Given time, we’d want to add a toggle for air flow level that the user can change.
    - Define how you will measure the quality / performance of your system (e.g. graphs showing speedup, or quantifying accuracy). It may not be possible to define precise target metrics at this time, but we encourage you to try.
        - We’ll measure quality by observing how realistic the motion and material render. Wind dynamics are ultimately pseudorandom, and so we cannot describe exactly what a series of input points would map to 
        
- What questions do you plan to answer with your analysis?
    - We hope to test our assumption that, if we were to define our particle physics correctly, and define our tube man cloth physics correctly, then the behavior of the tube man follows from the rules of these physics.
    
- You should break this section into two parts: (1) what you plan to deliver, and (2) what you hope to deliver. 
- In (1), **describe what you believe you must accomplish to have a successful project and achieve the grade you expect (i.e. your baseline plan -- planning for some unexpected problems would make sense)**
    - At minimum, we plan to accomplish basic cloth-wind interaction — for example, a flag waving through the wind would be our baseline. 
    - Ideally, our target is to render a 3D tube through which particles can realistically flow, while the material simultaneously interacts with the wind 


- In (2), **describe what you hope to achieve if things go well and you get ahead of schedule (your aspirational plan).**
    - FULL TUBE MAN
    - Our reach goal is to deliver a realistic looking, 3D inflatable tube man, using non-uniform air flow to simulate the jerky motion and independent hand movement.
    - We would love to, if we were to get ahead of schedule, give the user the ability to modify air flow rate in real-time, allowing people to experiment with different tube man motions and different air flows.


## Schedule
- In this section you should organize and plan the tasks and subtasks that your team will execute. Since presentations are ~4 weeks from the due-date of the proposal, you should include a set of tasks for every week.

**Week 1**

- **Finish the particle physics engine**, be able to simulate gas flow at different rates as well as wind
    - Deep dive into available particle physics engines, with emphasis on those written in C++ and those that are open-source, so that we can fork and make amendments where necessary.
    - We need to be sure about our ability to integrate the library we choose into ClothSim, or otherwise develop our own particle physics library in this week. A big focus of this deep dive will be the essential ability to integrate.  
- Start integration of wind physics into cloth simulator, potentially making amendments to the real-time rendering software for Project 4.
    - Understand Project 4’s real-time rendering engine to be able to integrate wind motion into it, potentially asking for help from course staff when necessary.

**Week 2**

- **Cloth and wind interaction**, in the style of flags waving through the air.
    - Completely integrate the particle engine into our cloth dynamics, to be able to make any subsequent tasks possible.
    - ClothSim already has the ability to define cloth motion fall using gravity — we consider building atop this with our particle engine to define cloth motion in the wind. 
- Start designing open tube using our cloth simulation for Week 3 tube/wind interaction.
- Start prepping for graded milestone submission

**Week 3**

- **Wind and open tube interaction**, managing air flow rate through tube and visual changes to air motion.
- Start designing tube man for Week 4 tube man/wind interaction.
- Submit milestone report
- Begin working on presentation 

**Week 4**

- **Pseudorandom wind and inflatable tube interaction**, defining jerky motion of the tube and air creating pressure by escaping through narrow slits.
- Deliver final presentation and wrap up final deliverables
## Resources
- List what resources, e.g. books, papers and/or online resources that are references for your project. List the computing platform, hardware and software resources that you will use for your project. You have a wide latitude here to use what you have access to, but be aware that you will have to support and trouble-shoot on your platform yourselves. If you are starting from an existing piece of code or system, describe and provide a pointer to it here.
![The Battle of a Life Time - Señor GIF - Pronounced GIF or JIF?](https://i.chzbgr.com/full/7895156992/hFDB8D40C/the-battle-of-a-life-time)

- https://josephml.com/wp-content/uploads/2018/07/first-attempts-model-1.pdf?189db0&189db0 
- https://github.com/waltomatt/particles

