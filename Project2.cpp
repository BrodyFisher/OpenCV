#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <vector>
#include <iostream>


using namespace cv;
using namespace std;

//Global because other functions use it lol
vector <Rect> faces;

void eye(Mat img, CascadeClassifier detect_eyes)
{
    vector <Rect> eyes;

    //detecting eyes
    Size minsize(30, 30);
    Size maxsize(100, 100);
    detect_eyes.detectMultiScale(img, eyes, 1.1, 10, 3, minsize, maxsize);

    for (int i = 0; i < eyes.size(); i++)
    {
        //center of each eye
        Point center(faces[0].x + (eyes[i].x - faces[0].x) + eyes[i].width/2, faces[0].y + (eyes[i].y - faces[0].y) + eyes[i].height/2);
        circle(img, center, eyes[i].width / 2, Scalar(255, 144, 34), 2);
    }
}
void smile(Mat img, CascadeClassifier detect_smile)
{
    vector <Rect> smiles;

    //detecting smile :) better go for a VERY neutral backgroud with perfect lighting because apparently everything is a smile
    Size minsize(40,40);
    Size maxsize(100, 100);
    detect_smile.detectMultiScale(img, smiles, 1.1, 10, 3, minsize, maxsize);
    if (smiles.size() > 0)
    {
        for (int i = 0; i < smiles.size(); i++)
        {
            Point center(faces[0].x + (smiles[i].x - faces[0].x) + smiles[i].width / 2, faces[0].y + (smiles[i].y - faces[0].y));

            //only drawing smile if its within the detected face because again, apparently everything is a face
            if (smiles[i].x > faces[0].x && smiles[i].y > faces[0].y && smiles[i].x < (faces[0].x + faces[0].width) && smiles[i].y < (faces[0].y + faces[0].height))
                ellipse(img, center, Size(smiles[i].width/2, smiles[i].height), 180, 180, 360, Scalar(255, 144, 34), FILLED);
        }
        cout << smiles.size() << "\n";
    }
    else //if no smile is detected tailored to me because I am me
    {
        Point center((faces[0].x + faces[0].width / 2), (faces[0].y + faces[0].height * 0.85));
        ellipse(img, center, Size(50, 35), 180, 0, 180, Scalar(255, 144, 34), FILLED); //upside down smile because no smile detected
    }
}

//Bool so that the program doesn't bother to look for the stuff on the face if there is no face
bool face(Mat img, CascadeClassifier detect_face)
{
    //pushing found faces into the faces vector
    detect_face.detectMultiScale(img, faces, 1.1, 10);

    if (faces.size() > 0)
    {
        for (int i = 0; i < faces.size(); i++)
        {
            //creating a center point
            Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
            circle(img, center, faces[i].height / 2, Scalar(255, 144, 34), 2);
            return true;
        }
    }
    else
    {
        return false;
    }
}

int main()
{
    //Obtaining webcam
    VideoCapture cap(0);

    //The display matrices
    Mat img;

    //loading the cascasde files to be able to detect the things :)
    CascadeClassifier detect_face;
    CascadeClassifier detect_eyes;
    CascadeClassifier detect_smile;

    detect_face.load("Resources/haarcascade_frontalface_default.xml");
    detect_eyes.load("Resources/haarcascade_eye.xml");
    detect_smile.load("Resources/haarcascade_smile.xml");

    while (true)
    {
        cap.read(img);

        //face things
        if (face(img, detect_face))
        {
            eye(img, detect_eyes);
            smile(img, detect_smile);
        }

        imshow("New face filters on instagram", img);

        waitKey(1);
    }
    return 0;
}