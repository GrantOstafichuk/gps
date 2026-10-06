#include "tinyxml2.h"
#include "TrekPoint.h"
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>


std::vector<TrekPoint> readGPXFile(const char filename[])
{

    std::vector<TrekPoint> res;

    // read in our gpx file -----
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError result = doc.LoadFile(filename);

    if (result != tinyxml2::XML_SUCCESS)
    {
        std::cerr << "Could not read the file\n";
        return res;
    }
    // -----

    // check that it is a .gpx file -----
    tinyxml2::XMLElement* gpx = doc.FirstChildElement("gpx");

    if (gpx == nullptr)
    {
        std::cerr << "No <gpx> element found\n";
        return res;
    }
    // ------


    // store meta data -----
    auto metadata = gpx->FirstChildElement("metadata");
   
    if (metadata)
    {
        std::cout << "Metadata:\n";

        auto name = gpx->FirstChildElement("name");

        if (name && name->GetText())
        {
            std::cout << " Name: " << name->GetText() << "\n";
        }
        auto desc = metadata->FirstChildElement("desc");

        if (desc && desc->GetText())
        {
            std::cout << "  Description: " << desc->GetText() << '\n';
        }

        auto time = metadata->FirstChildElement("time");

        if (time && time->GetText())
        {
            std::cout << "  Time: " << time->GetText() << '\n';
        }
            

    }
    // -----------------------------

    // look at the waypoints now -----
    auto waypoint = gpx->FirstChildElement("wpt");

    while (waypoint)
    {
        double lat = waypoint->DoubleAttribute("lat");
        double lon = waypoint->DoubleAttribute("lon");

        std::cout << "Waypoint:\n";
        std::cout << "  Lat: " << lat << '\n';
        std::cout << "  Lon: " << lon << '\n';

        waypoint = waypoint->NextSiblingElement("wpt");
    }
    // -------------------------------

    // trkseg -----
    // should be 3 name, sourse and trkpt
   
    // make a list of all our trek points ------
    auto trk = gpx->FirstChildElement("trk");
    auto trkseg = trk->FirstChildElement("trkseg");
    auto trkpt = trkseg->FirstChildElement("trkpt");

    while (trkpt)
    {
        auto child = trkpt->FirstChildElement();

        if (child)
        {

            if (!child->GetText())
                std::cerr << "Elevation missing in the .gpx file\n";
            
            TrekPoint trkPt;
            trkPt.elevation = std::stod(child->GetText());
            trkPt.latitude =  trkpt->DoubleAttribute("lat");
            trkPt.longitude =  trkpt->DoubleAttribute("lon");

            std::cout << "Elevation = " << trkPt.elevation;
            std::cout << " Latitiude = " << trkPt.latitude;
            std::cout << " Longitude = " << trkPt.longitude;
            std::cout << "\n";
 
            res.push_back(trkPt);
        }

        trkpt = trkpt->NextSiblingElement();
    }
    // -----------------------------------------

    

    // ----------------------------------------------------------------------------------

    return res;
}

void exportCSV(const char filename[], std::vector<TrekPoint> trkPts)
{
    // simple code to export to .csv file
    std::ofstream file(filename);

    file << "latitude,longitude,elevation\n";

    file << std::setprecision(15);

    for (const auto& point : trkPts)
    {
        file << point.latitude << ","
            << point.longitude << ","
            << point.elevation << "\n";
    }

    file.close();
}