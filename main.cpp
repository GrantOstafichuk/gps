#include "tinyxml2.h"
#include <iostream>

int main(int argc, char* argv[])
{

    // read in our gpx file -----
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <filename.gpx>\n";
        return 1;
    }


    tinyxml2::XMLDocument doc;

    tinyxml2::XMLError result = doc.LoadFile(argv[1]);

    if (result != tinyxml2::XML_SUCCESS)
    {
        std::cout << "Count not read the file\n";
        return 1;
    }

    std::cout << "FILE READED\n";
    // -----

    // check that it is a .gpx file -----
    tinyxml2::XMLElement* gpx = doc.FirstChildElement("gpx");

    if (gpx == nullptr)
    {
        std::cerr << "No <gpx> element found\n";
        return 1;
    }
    // ------

    // extract all the important data from our doc
    auto author = gpx->Attribute("creator");
    auto version = gpx->Attribute("version");

    std::cout << "Author " << author << "\n";
    std::cout << "version " << version << "\n";
    // -----

    // waypoint count-----
    auto waypoints = gpx->ChildElementCount() - 2;
    std::cout << "Child element count of gpx" << waypoints << "\n";
    // ------

    // remove meta data -----
    gpx->DeleteChild(gpx->FirstChild());
    // ----------------------

    // all waypoints in a list -----
    while (waypoints > 0)
    {
        auto next = gpx->FirstChildElement();
        std::cout << next->FirstAttribute()->Value() << "\n";
        gpx->DeleteChild(gpx->FirstChild());

        waypoints--;
    }
    // -----------------------------

    // trkseg -----
    // should be 3 name, sourse and trkpt
    auto trk = gpx->FirstChild();
    std::cout << "Elements under trk " << trk->ChildElementCount() << "\n";

    // remove name and src
    trk->DeleteChild(trk->FirstChild());
    trk->DeleteChild(trk->FirstChild());

    // trek point count
    auto trkseg = trk->FirstChild();
    std::cout << "Number of trekpoints " << trkseg->ChildElementCount() << "\n";
    // --------------------------

    // make a list of all our trek points ------
    auto trkpt = trkseg->FirstChildElement();
    while (trkpt)
    {

        // lets print our trek points first
        std::cout << trkpt->Value() << "\n";

        auto child = trkpt->FirstChildElement();

        while (child)
        {
            std::cout << "  " << trkpt->DoubleAttribute("lat");
            std::cout << "  " << trkpt->DoubleAttribute("lon");
            std::cout << "  " << child->Value();

            if (child->GetText())
            {
                std::cout << " = " << child->GetText();
            }

            std::cout << "\n";

            child = child->NextSiblingElement();
        }

        trkpt = trkpt->NextSiblingElement();
    }
    // -----------------------------------------


    return 0;
}