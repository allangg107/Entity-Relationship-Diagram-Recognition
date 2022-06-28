# Entity Relationship Diagram Recognition Project
This was a final project for a computer vision course our group took.
Below is the write up of our project.

# What We Tried to Accomplish:
Our main project was for Entity Relationship (ER) Diagram recognition. 
The goal of our program was to recognize and identify the various symbols in a hand drawn ER diagram. 
The program would then output a copy of the diagram that is appropriately labeled.

# What We Did Accomplish:
We accomplished fairly accurately recognizing 6 different symbols from hand drawn ER
diagrams (attributes, multivalued attributes, entities, weak entities, relationships, and
weak relationships). Occasionally relationships and total participation are recognized as
entities (which they look similar), but if the diagram is drawn with a reasonable degree
of quality the program is very accurate.

# Results:
Given an input diagram (created in MS Paint), our program finds all the contours in the
image, analyzes those contours, and labels each contour as the appropriate symbol on
a copy of the diagram. Here are some of the resulting outputs of our program:

Basic Example:

<img width="453" alt="Capture" src="https://user-images.githubusercontent.com/76569535/176110601-00b9246c-f43a-4e0c-8aac-b88d2d2100f2.PNG">

Intermediate Example:

<img width="385" alt="Capture2" src="https://user-images.githubusercontent.com/76569535/176110647-095bad18-966f-43e4-8c5f-c10968af3c10.PNG">

Advanced Example:

<img width="377" alt="Capture3" src="https://user-images.githubusercontent.com/76569535/176110671-e3dfd39a-96f6-490d-9a45-1571a031ffa8.PNG">

# Lessons Learned:
● Learned about how certain opencv methods work (mainly methods revolving around contours).

● Different drawing applications can produce different contours even if visually the image looks the same.

● Total participation sometimes gets interpreted as an entity

# Future Improvements:
With more time, the following features could be added:

● Handle more diagram symbols (e.g, total participation, composite attributes, etc.)

● Handle non-diagram symbols (e.g, triangles, pentagons, etc.)

● Handle severing (e.g, a rectangle that isn't closed or lines that aren't connected)

● Increase robustness (e.g, to avoid misclassification of total participation as an entity)

● Recognizing which types are related (e.g, entity1 has relationship with entity2)

● Construct resulting relational tables (this would require recognizing text)

# Documentation on code we didn’t use:
We have two methods that weren’t used in our implementation, checkIfWeak() and
angle(). This documentation is to record effort put towards alternative solutions that
were explored.

checkifWeak() was a method to check if an entity (rectangle) or relationship
(square/diamond) had children in order to determine if it was a weak type. This would
take advantage of opencv’s hierarchy identification feature. But after doing research on
the hierarchy vector that is generated with findContour(), we ran into issues such as
contours that shouldn’t have a child, had a child. After hours researching and
configuring we abandoned this approach.

angle() was a method to check the angle of a shape. After identifying that a shape is
enclosed, it would determine if the shape was between a rectangle, pentagon, or a
hexagon. But we determine that our program would only consider ER diagram shapes
and assume that the test images do not have any unidentifiable shapes. If we add the
feature to be able to recognize non-diagram symbols, the angle() method would be
utilized in the implementation of it.

# Sources:
How to detect a color:

● https://stackoverflow.com/questions/32522989/opencv-better-detection-ofred-color

ER Diagram info:

● https://www.lucidchart.com/pages/er-diagrams/#section_0

● https://www.logianalytics.com/relational-vs-non-relational-databases/

Contour Detection:

● https://learnopencv.com/contour-detection-using-opencv-python-c/

Detect Shapes:

● https://www.geeksforgeeks.org/how-to-detect-shapes-in-images-in-pythonusing-opencv/amp/

Finding/Labeling Shapes:

● http://www.calumk.com/old_posts_archive/0008//detecting-simple-shapesin-an-image/

● https://www.geeksforgeeks.org/how-to-detect-shapes-in-images-in-pythonusing-opencv/

Approximation Contour:

● https://docs.opencv.org/3.4/dd/d49/tutorial_py_contour_features.html

Squares.cpp:

● https://docs.opencv.org/4.x/db/d00/samples_2cpp_2squares_8cpp-example.html

Hierarchy explanation:

● https://stackoverflow.com/questions/23012623/opencv-drawing-contours-as-they-are

Unsevered blobs:

● https://answers.opencv.org/question/25183/is-there-an-opencv-methodfunction-to-detect-unsevered-blobs/#25223
