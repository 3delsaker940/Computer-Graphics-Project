#include "Room.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace Example
{
    void Room::create(std::string roomName, glm::vec3 position, float roomSize, glm::vec3 color, bool isDoorAtMaxZ)
    {
        this->name = roomName;
        this->centerOffset = position;
        this->size = roomSize;
        this->doorAtMaxZ = isDoorAtMaxZ;

        float h = 7.0f;
        this->doorWidth = 5.0f;
        this->doorHeight = 4.5f;

        std::vector<BasicVertex> floorV, wallV, lightV, ceilingV;
        auto addPos = [&](glm::vec3 p) { return p + centerOffset; };

                        
        auto addQuadWithNormal = [](std::vector<BasicVertex>& verts,
            glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4,
            glm::vec3 col)
            {
                glm::vec3 edge1 = p2 - p1;
                glm::vec3 edge2 = p3 - p1;
                glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

                verts.push_back({ p1, col, normal });
                verts.push_back({ p2, col, normal });
                verts.push_back({ p3, col, normal });
                verts.push_back({ p1, col, normal });
                verts.push_back({ p3, col, normal });
                verts.push_back({ p4, col, normal });
            };

        auto addQuadLocal = [&](std::vector<BasicVertex>& v,
            glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4,
            glm::vec3 col)
            {
                glm::vec3 edge1 = p2 - p1;
                glm::vec3 edge2 = p3 - p1;
                glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

                v.push_back({ addPos(p1), col, normal });
                v.push_back({ addPos(p2), col, normal });
                v.push_back({ addPos(p3), col, normal });
                v.push_back({ addPos(p1), col, normal });
                v.push_back({ addPos(p3), col, normal });
                v.push_back({ addPos(p4), col, normal });
            };

        auto addBoxLocal = [&](std::vector<BasicVertex>& v,
            glm::vec3 mn, glm::vec3 mx, glm::vec3 col)
            {
                glm::vec3 a(mn.x, mn.y, mn.z);
                glm::vec3 b(mx.x, mn.y, mn.z);
                glm::vec3 c(mx.x, mx.y, mn.z);
                glm::vec3 d(mn.x, mx.y, mn.z);
                glm::vec3 e(mn.x, mn.y, mx.z);
                glm::vec3 f(mx.x, mn.y, mx.z);
                glm::vec3 g(mx.x, mx.y, mx.z);
                glm::vec3 hh(mn.x, mx.y, mx.z);

                addQuadLocal(v, a, b, c, d, col);
                addQuadLocal(v, f, e, hh, g, col);
                addQuadLocal(v, e, a, d, hh, col);
                addQuadLocal(v, b, f, g, c, col);
                addQuadLocal(v, d, c, g, hh, col);
                addQuadLocal(v, e, f, b, a, col);
            };

                        
        glm::vec3 accentColor = { 1.0f, 1.0f, 1.0f };
        glm::vec3 floorColor1, floorColor2;
        glm::vec3 baseboardColor;

        if (roomName == "Luxury")
        {
            accentColor = { 0.85f, 0.70f, 0.25f };    
            floorColor1 = { 0.12f, 0.12f, 0.14f };     
            floorColor2 = { 0.18f, 0.18f, 0.20f };      
            baseboardColor = { 0.08f, 0.08f, 0.10f };
        }
        else if (roomName == "Electric")
        {
            accentColor = { 0.2f, 0.85f, 1.0f };       
            
            floorColor1 = { 0.08f, 0.10f, 0.15f };      
            floorColor2 = { 0.12f, 0.14f, 0.20f };
            baseboardColor = { 0.05f, 0.08f, 0.12f };
        }
        else if (roomName == "Sports")
        {
            accentColor = { 0.95f, 0.15f, 0.15f };    
            floorColor1 = { 0.10f, 0.08f, 0.08f };   
            floorColor2 = { 0.15f, 0.12f, 0.12f };
            baseboardColor = { 0.08f, 0.06f, 0.06f };
        }
        else if (roomName == "Family")
        {
            accentColor = { 0.4f, 0.85f, 0.4f };           
            floorColor1 = { 0.12f, 0.10f, 0.08f };         
            floorColor2 = { 0.18f, 0.15f, 0.12f };
            baseboardColor = { 0.10f, 0.08f, 0.06f };
        }
        else
        {
            floorColor1 = color * 0.25f;
            floorColor2 = color * 0.35f;
            baseboardColor = color * 0.15f;
        }

                        
        glm::vec3 upNormal = { 0, 1, 0 };
        float tileSize = 3.0f;         
        int tilesCount = static_cast<int>(size * 2 / tileSize);

        for (int i = 0; i < tilesCount; i++)
        {
            for (int j = 0; j < tilesCount; j++)
            {
                float x1 = -size + i * tileSize;
                float z1 = -size + j * tileSize;
                float x2 = x1 + tileSize;
                float z2 = z1 + tileSize;


                               
                x2 = std::min(x2, size);
                z2 = std::min(z2, size);

                             
                bool isDark = (i + j) % 2 == 0;
                glm::vec3 tileColor = isDark ? floorColor1 : floorColor2;

                             
                float variation = ((i * 7 + j * 13) % 100) / 1000.0f;
                tileColor += glm::vec3(variation, variation, variation);

                floorV.push_back({ addPos({x1, 0.001f, z1}), tileColor, upNormal });
                floorV.push_back({ addPos({x2, 0.001f, z1}), tileColor, upNormal });
                floorV.push_back({ addPos({x2, 0.001f, z2}), tileColor, upNormal });
                floorV.push_back({ addPos({x1, 0.001f, z1}), tileColor, upNormal });
                floorV.push_back({ addPos({x2, 0.001f, z2}), tileColor, upNormal });
                floorV.push_back({ addPos({-size + i * tileSize, 0.001f, z2}), tileColor, upNormal });
            }
        }

             
        glm::vec3 groutColor = floorColor1 * 0.5f;
        float groutWidth = 0.05f;

        for (int i = 0; i <= tilesCount; i++)
        {
            float x = -size + i * tileSize;
            if (x > size) x = size;

                      
            floorV.push_back({ addPos({x - groutWidth, 0.002f, -size}), groutColor, upNormal });
            floorV.push_back({ addPos({x + groutWidth, 0.002f, -size}), groutColor, upNormal });
            floorV.push_back({ addPos({x + groutWidth, 0.002f, size}), groutColor, upNormal });
            floorV.push_back({ addPos({x - groutWidth, 0.002f, -size}), groutColor, upNormal });
            floorV.push_back({ addPos({x + groutWidth, 0.002f, size}), groutColor, upNormal });
            floorV.push_back({ addPos({x - groutWidth, 0.002f, size}), groutColor, upNormal });
        }

        for (int j = 0; j <= tilesCount; j++)
        {
            float z = -size + j * tileSize;
            if (z > size) z = size;

                       
            floorV.push_back({ addPos({-size, 0.002f, z - groutWidth}), groutColor, upNormal });
            floorV.push_back({ addPos({size, 0.002f, z - groutWidth}), groutColor, upNormal });
            floorV.push_back({ addPos({size, 0.002f, z + groutWidth}), groutColor, upNormal });
            floorV.push_back({ addPos({-size, 0.002f, z - groutWidth}), groutColor, upNormal });
            floorV.push_back({ addPos({size, 0.002f, z + groutWidth}), groutColor, upNormal });
            floorV.push_back({ addPos({-size, 0.002f, z + groutWidth}), groutColor, upNormal });
        }

        floor = BasicShape(floorV);

                        
        float pW = 4.2f;
        float pD = 5.2f;
        float pH = 0.39f;
        float spacing = 10.5f;
        float pZ = isDoorAtMaxZ ? -size * 0.35f : size * 0.35f;

               
        glm::vec3 pTopColor = { 0.20f, 0.20f, 0.22f };
        glm::vec3 pSideColor = { 0.12f, 0.12f, 0.14f }; 
        glm::vec3 pEdgeColor = { 0.08f, 0.08f, 0.10f };     
        glm::vec3 ledColor = accentColor;                   
        glm::vec3 nUp = { 0, 1, 0 };
        glm::vec3 nFront = { 0, 0, 1 };
        glm::vec3 nBack = { 0, 0, -1 };
        glm::vec3 nLeft = { -1, 0, 0 };
        glm::vec3 nRight = { 1, 0, 0 };

        float podiumPositions[3] = { -spacing, 0.0f, spacing };

        podiums.clear();
        podiumBounds.clear();

        for (int i = 0; i < 3; i++)
        {
            float pX = podiumPositions[i];
            std::vector<BasicVertex> podiumV;

                                    
                        float borderW = 0.15f;
            glm::vec3 borderColor = pTopColor * 0.7f;

                      
            podiumV.push_back({ addPos({pX - pW + borderW, pH, pZ - pD + borderW}), pTopColor, nUp });
            podiumV.push_back({ addPos({pX + pW - borderW, pH, pZ - pD + borderW}), pTopColor, nUp });
            podiumV.push_back({ addPos({pX + pW - borderW, pH, pZ + pD - borderW}), pTopColor, nUp });
            podiumV.push_back({ addPos({pX - pW + borderW, pH, pZ - pD + borderW}), pTopColor, nUp });
            podiumV.push_back({ addPos({pX + pW - borderW, pH, pZ + pD - borderW}), pTopColor, nUp });
            podiumV.push_back({ addPos({pX - pW + borderW, pH, pZ + pD - borderW}), pTopColor, nUp });

                                  
            podiumV.push_back({ addPos({pX - pW, pH, pZ + pD - borderW}), borderColor, nUp });
            podiumV.push_back({ addPos({pX + pW, pH, pZ + pD - borderW}), borderColor, nUp });
            podiumV.push_back({ addPos({pX + pW, pH, pZ + pD}), borderColor, nUp });
            podiumV.push_back({ addPos({pX - pW, pH, pZ + pD - borderW}), borderColor, nUp });
            podiumV.push_back({ addPos({pX + pW, pH, pZ + pD}), borderColor, nUp });
            podiumV.push_back({ addPos({pX - pW, pH, pZ + pD}), borderColor, nUp });

                       
            podiumV.push_back({ addPos({pX - pW, pH, pZ - pD}), borderColor, nUp });
            podiumV.push_back({ addPos({pX + pW, pH, pZ - pD}), borderColor, nUp });
            podiumV.push_back({ addPos({pX + pW, pH, pZ - pD + borderW}), borderColor, nUp });
            podiumV.push_back({ addPos({pX - pW, pH, pZ - pD}), borderColor, nUp });
            podiumV.push_back({ addPos({pX + pW, pH, pZ - pD + borderW}), borderColor, nUp });
            podiumV.push_back({ addPos({pX - pW, pH, pZ - pD + borderW}), borderColor, nUp });

                      
            podiumV.push_back({ addPos({pX - pW, pH, pZ - pD}), borderColor, nUp });
            podiumV.push_back({ addPos({pX - pW + borderW, pH, pZ - pD}), borderColor, nUp });
            podiumV.push_back({ addPos({pX - pW + borderW, pH, pZ + pD}), borderColor, nUp });
            podiumV.push_back({ addPos({pX - pW, pH, pZ - pD}), borderColor, nUp });
            podiumV.push_back({ addPos({pX - pW + borderW, pH, pZ + pD}), borderColor, nUp });
            podiumV.push_back({ addPos({pX - pW, pH, pZ + pD}), borderColor, nUp });

                     
            podiumV.push_back({ addPos({pX + pW - borderW, pH, pZ - pD}), borderColor, nUp });
            podiumV.push_back({ addPos({pX + pW, pH, pZ - pD}), borderColor, nUp });
            podiumV.push_back({ addPos({pX + pW, pH, pZ + pD}), borderColor, nUp });
            podiumV.push_back({ addPos({pX + pW - borderW, pH, pZ - pD}), borderColor, nUp });
            podiumV.push_back({ addPos({pX + pW, pH, pZ + pD}), borderColor, nUp });
            podiumV.push_back({ addPos({pX + pW - borderW, pH, pZ + pD}), borderColor, nUp });

                                    
                        podiumV.push_back({ addPos({pX - pW, 0,  pZ + pD}), pSideColor, nFront });
            podiumV.push_back({ addPos({pX + pW, 0,  pZ + pD}), pSideColor, nFront });
            podiumV.push_back({ addPos({pX + pW, pH, pZ + pD}), pEdgeColor, nFront });
            podiumV.push_back({ addPos({pX - pW, 0,  pZ + pD}), pSideColor, nFront });
            podiumV.push_back({ addPos({pX + pW, pH, pZ + pD}), pEdgeColor, nFront });
            podiumV.push_back({ addPos({pX - pW, pH, pZ + pD}), pEdgeColor, nFront });

                      
            podiumV.push_back({ addPos({pX + pW, 0,  pZ - pD}), pSideColor, nBack });
            podiumV.push_back({ addPos({pX - pW, 0,  pZ - pD}), pSideColor, nBack });
            podiumV.push_back({ addPos({pX - pW, pH, pZ - pD}), pEdgeColor, nBack });
            podiumV.push_back({ addPos({pX + pW, 0,  pZ - pD}), pSideColor, nBack });
            podiumV.push_back({ addPos({pX - pW, pH, pZ - pD}), pEdgeColor, nBack });
            podiumV.push_back({ addPos({pX + pW, pH, pZ - pD}), pEdgeColor, nBack });

                      
            podiumV.push_back({ addPos({pX - pW, 0,  pZ - pD}), pSideColor, nLeft });
            podiumV.push_back({ addPos({pX - pW, 0,  pZ + pD}), pSideColor, nLeft });
            podiumV.push_back({ addPos({pX - pW, pH, pZ + pD}), pEdgeColor, nLeft });
            podiumV.push_back({ addPos({pX - pW, 0,  pZ - pD}), pSideColor, nLeft });
            podiumV.push_back({ addPos({pX - pW, pH, pZ + pD}), pEdgeColor, nLeft });
            podiumV.push_back({ addPos({pX - pW, pH, pZ - pD}), pEdgeColor, nLeft });

                        
            podiumV.push_back({ addPos({pX + pW, 0,  pZ + pD}), pSideColor, nRight });
            podiumV.push_back({ addPos({pX + pW, 0,  pZ - pD}), pSideColor, nRight });
            podiumV.push_back({ addPos({pX + pW, pH, pZ - pD}), pEdgeColor, nRight });
            podiumV.push_back({ addPos({pX + pW, 0,  pZ + pD}), pSideColor, nRight });
            podiumV.push_back({ addPos({pX + pW, pH, pZ - pD}), pEdgeColor, nRight });
            podiumV.push_back({ addPos({pX + pW, pH, pZ + pD}), pEdgeColor, nRight });

                                    
            float ledH = 0.08f;     
            
            float ledOffset = 0.02f;   
            glm::vec3 ledBright = ledColor * 1.5f;  
                    
            podiumV.push_back({ addPos({pX - pW - ledOffset, ledOffset, pZ + pD + ledOffset}), ledColor, nFront });
            podiumV.push_back({ addPos({pX + pW + ledOffset, ledOffset, pZ + pD + ledOffset}), ledColor, nFront });
            podiumV.push_back({ addPos({pX + pW + ledOffset, ledH, pZ + pD + ledOffset}), ledBright, nFront });
            podiumV.push_back({ addPos({pX - pW - ledOffset, ledOffset, pZ + pD + ledOffset}), ledColor, nFront });
            podiumV.push_back({ addPos({pX + pW + ledOffset, ledH, pZ + pD + ledOffset}), ledBright, nFront });
            podiumV.push_back({ addPos({pX - pW - ledOffset, ledH, pZ + pD + ledOffset}), ledBright, nFront });

                    
            podiumV.push_back({ addPos({pX + pW + ledOffset, ledOffset, pZ - pD - ledOffset}), ledColor, nBack });
            podiumV.push_back({ addPos({pX - pW - ledOffset, ledOffset, pZ - pD - ledOffset}), ledColor, nBack });
            podiumV.push_back({ addPos({pX - pW - ledOffset, ledH, pZ - pD - ledOffset}), ledBright, nBack });
            podiumV.push_back({ addPos({pX + pW + ledOffset, ledOffset, pZ - pD - ledOffset}), ledColor, nBack });
            podiumV.push_back({ addPos({pX - pW - ledOffset, ledH, pZ - pD - ledOffset}), ledBright, nBack });
            podiumV.push_back({ addPos({pX + pW + ledOffset, ledH, pZ - pD - ledOffset}), ledBright, nBack });

                    
            podiumV.push_back({ addPos({pX - pW - ledOffset, ledOffset, pZ - pD - ledOffset}), ledColor, nLeft });
            podiumV.push_back({ addPos({pX - pW - ledOffset, ledOffset, pZ + pD + ledOffset}), ledColor, nLeft });
            podiumV.push_back({ addPos({pX - pW - ledOffset, ledH, pZ + pD + ledOffset}), ledBright, nLeft });
            podiumV.push_back({ addPos({pX - pW - ledOffset, ledOffset, pZ - pD - ledOffset}), ledColor, nLeft });
            podiumV.push_back({ addPos({pX - pW - ledOffset, ledH, pZ + pD + ledOffset}), ledBright, nLeft });
            podiumV.push_back({ addPos({pX - pW - ledOffset, ledH, pZ - pD - ledOffset}), ledBright, nLeft });

                     
            podiumV.push_back({ addPos({pX + pW + ledOffset, ledOffset, pZ + pD + ledOffset}), ledColor, nRight });
            podiumV.push_back({ addPos({pX + pW + ledOffset, ledOffset, pZ - pD - ledOffset}), ledColor, nRight });
            podiumV.push_back({ addPos({pX + pW + ledOffset, ledH, pZ - pD - ledOffset}), ledBright, nRight });
            podiumV.push_back({ addPos({pX + pW + ledOffset, ledOffset, pZ + pD + ledOffset}), ledColor, nRight });
            podiumV.push_back({ addPos({pX + pW + ledOffset, ledH, pZ - pD - ledOffset}), ledBright, nRight });
            podiumV.push_back({ addPos({pX + pW + ledOffset, ledH, pZ + pD + ledOffset}), ledBright, nRight });

            podiums.push_back(BasicShape(podiumV));

            podiumBounds.push_back({
                centerOffset.x + pX,
                centerOffset.z + pZ,
                pW,
                pD
                });
        }

                        
        glm::vec3 lCol = { 1.0f, 1.0f, 0.95f };
        glm::vec3 lColBright = { 1.2f, 1.2f, 1.1f };
        glm::vec3 downNormal = { 0, -1, 0 };
        float lS = size * 0.35f;
        float lY = h - 0.01f;

               
        lightV.push_back({ addPos({-lS, lY, -lS}), lCol, downNormal });
        lightV.push_back({ addPos({lS, lY, -lS}), lCol, downNormal });
        lightV.push_back({ addPos({lS, lY, lS}), lCol, downNormal });
        lightV.push_back({ addPos({-lS, lY, -lS}), lCol, downNormal });
        lightV.push_back({ addPos({lS, lY, lS}), lCol, downNormal });
        lightV.push_back({ addPos({-lS, lY, lS}), lCol, downNormal });

               
        float frameW = 0.3f;
        glm::vec3 frameGlow = accentColor * 0.8f;

              
        lightV.push_back({ addPos({-lS - frameW, lY, lS}), frameGlow, downNormal });
        lightV.push_back({ addPos({lS + frameW, lY, lS}), frameGlow, downNormal });
        lightV.push_back({ addPos({lS + frameW, lY, lS + frameW}), frameGlow, downNormal });
        lightV.push_back({ addPos({-lS - frameW, lY, lS}), frameGlow, downNormal });
        lightV.push_back({ addPos({lS + frameW, lY, lS + frameW}), frameGlow, downNormal });
        lightV.push_back({ addPos({-lS - frameW, lY, lS + frameW}), frameGlow, downNormal });

               
        lightV.push_back({ addPos({-lS - frameW, lY, -lS - frameW}), frameGlow, downNormal });
        lightV.push_back({ addPos({lS + frameW, lY, -lS - frameW}), frameGlow, downNormal });
        lightV.push_back({ addPos({lS + frameW, lY, -lS}), frameGlow, downNormal });
        lightV.push_back({ addPos({-lS - frameW, lY, -lS - frameW}), frameGlow, downNormal });
        lightV.push_back({ addPos({lS + frameW, lY, -lS}), frameGlow, downNormal });
        lightV.push_back({ addPos({-lS - frameW, lY, -lS}), frameGlow, downNormal });

            
        lightV.push_back({ addPos({-lS - frameW, lY, -lS}), frameGlow, downNormal });
        lightV.push_back({ addPos({-lS, lY, -lS}), frameGlow, downNormal });
        lightV.push_back({ addPos({-lS, lY, lS}), frameGlow, downNormal });
        lightV.push_back({ addPos({-lS - frameW, lY, -lS}), frameGlow, downNormal });
        lightV.push_back({ addPos({-lS, lY, lS}), frameGlow, downNormal });
        lightV.push_back({ addPos({-lS - frameW, lY, lS}), frameGlow, downNormal });

            
        lightV.push_back({ addPos({lS, lY, -lS}), frameGlow, downNormal });
        lightV.push_back({ addPos({lS + frameW, lY, -lS}), frameGlow, downNormal });
        lightV.push_back({ addPos({lS + frameW, lY, lS}), frameGlow, downNormal });
        lightV.push_back({ addPos({lS, lY, -lS}), frameGlow, downNormal });
        lightV.push_back({ addPos({lS + frameW, lY, lS}), frameGlow, downNormal });
        lightV.push_back({ addPos({lS, lY, lS}), frameGlow, downNormal });

        ceilingLight = BasicShape(lightV);

                        
        float baseboardH = 0.25f;  
                addQuadWithNormal(wallV,
            addPos({ -size, baseboardH, -size }), addPos({ -size, baseboardH, size }),
            addPos({ -size, h, size }), addPos({ -size, h, -size }),
            color);

                addQuadWithNormal(wallV,
            addPos({ -size, 0, -size }), addPos({ -size, 0, size }),
            addPos({ -size, baseboardH, size }), addPos({ -size, baseboardH, -size }),
            baseboardColor);

                addQuadWithNormal(wallV,
            addPos({ size, baseboardH, size }), addPos({ size, baseboardH, -size }),
            addPos({ size, h, -size }), addPos({ size, h, size }),
            color);

                addQuadWithNormal(wallV,
            addPos({ size, 0, size }), addPos({ size, 0, -size }),
            addPos({ size, baseboardH, -size }), addPos({ size, baseboardH, size }),
            baseboardColor);

                float doorZ = isDoorAtMaxZ ? size : -size;
        float solidZ = isDoorAtMaxZ ? -size : size;

        addQuadWithNormal(wallV,
            addPos({ -size, baseboardH, solidZ }), addPos({ size, baseboardH, solidZ }),
            addPos({ size, h, solidZ }), addPos({ -size, h, solidZ }),
            color);

                addQuadWithNormal(wallV,
            addPos({ -size, 0, solidZ }), addPos({ size, 0, solidZ }),
            addPos({ size, baseboardH, solidZ }), addPos({ -size, baseboardH, solidZ }),
            baseboardColor);

                glm::vec3 doorWallNormal = isDoorAtMaxZ ? glm::vec3(0, 0, -1) : glm::vec3(0, 0, 1);

                addQuadWithNormal(wallV,
            addPos({ -size, baseboardH, doorZ }), addPos({ -this->doorWidth / 2, baseboardH, doorZ }),
            addPos({ -this->doorWidth / 2, h, doorZ }), addPos({ -size, h, doorZ }),
            color);

                addQuadWithNormal(wallV,
            addPos({ -size, 0, doorZ }), addPos({ -this->doorWidth / 2, 0, doorZ }),
            addPos({ -this->doorWidth / 2, baseboardH, doorZ }), addPos({ -size, baseboardH, doorZ }),
            baseboardColor);

                addQuadWithNormal(wallV,
            addPos({ this->doorWidth / 2, baseboardH, doorZ }), addPos({ size, baseboardH, doorZ }),
            addPos({ size, h, doorZ }), addPos({ this->doorWidth / 2, h, doorZ }),
            color);

                addQuadWithNormal(wallV,
            addPos({ this->doorWidth / 2, 0, doorZ }), addPos({ size, 0, doorZ }),
            addPos({ size, baseboardH, doorZ }), addPos({ this->doorWidth / 2, baseboardH, doorZ }),
            baseboardColor);

                addQuadWithNormal(wallV,
            addPos({ -this->doorWidth / 2, this->doorHeight, doorZ }),
            addPos({ this->doorWidth / 2, this->doorHeight, doorZ }),
            addPos({ this->doorWidth / 2, h, doorZ }),
            addPos({ -this->doorWidth / 2, h, doorZ }),
            color);

                        
        float ledStripH = 0.12f;
        float ledStripY = h - 0.5f;
        glm::vec3 wallLedColor = accentColor * 0.6f;

                wallV.push_back({ addPos({-size + 0.01f, ledStripY, -size}), wallLedColor, {1, 0, 0} });
        wallV.push_back({ addPos({-size + 0.01f, ledStripY, size}), wallLedColor, {1, 0, 0} });
        wallV.push_back({ addPos({-size + 0.01f, ledStripY + ledStripH, size}), wallLedColor * 1.5f, {1, 0, 0} });
        wallV.push_back({ addPos({-size + 0.01f, ledStripY, -size}), wallLedColor, {1, 0, 0} });
        wallV.push_back({ addPos({-size + 0.01f, ledStripY + ledStripH, size}), wallLedColor * 1.5f, {1, 0, 0} });
        wallV.push_back({ addPos({-size + 0.01f, ledStripY + ledStripH, -size}), wallLedColor * 1.5f, {1, 0, 0} });

                wallV.push_back({ addPos({size - 0.01f, ledStripY, size}), wallLedColor, {-1, 0, 0} });
        wallV.push_back({ addPos({size - 0.01f, ledStripY, -size}), wallLedColor, {-1, 0, 0} });
        wallV.push_back({ addPos({size - 0.01f, ledStripY + ledStripH, -size}), wallLedColor * 1.5f, {-1, 0, 0} });
        wallV.push_back({ addPos({size - 0.01f, ledStripY, size}), wallLedColor, {-1, 0, 0} });
        wallV.push_back({ addPos({size - 0.01f, ledStripY + ledStripH, -size}), wallLedColor * 1.5f, {-1, 0, 0} });
        wallV.push_back({ addPos({size - 0.01f, ledStripY + ledStripH, size}), wallLedColor * 1.5f, {-1, 0, 0} });

        walls = BasicShape(wallV);

                        
        float doorHeight = 4.5f;
        glm::vec3 doorColor = color * 0.85f;

        std::vector<BasicVertex> doorV;
        glm::vec3 nDoor = { 0, 0, 1 };

        doorV.push_back({ {0.0f,       0.0f,      0.0f}, doorColor, nDoor });
        doorV.push_back({ {this->doorWidth,  0.0f,      0.0f}, doorColor, nDoor });
        doorV.push_back({ {this->doorWidth,  doorHeight,0.0f}, doorColor, nDoor });
        doorV.push_back({ {0.0f,       0.0f,      0.0f}, doorColor, nDoor });
        doorV.push_back({ {this->doorWidth,  doorHeight,0.0f}, doorColor, nDoor });
        doorV.push_back({ {0.0f,       doorHeight,0.0f}, doorColor, nDoor });

        doorPanel = BasicShape(doorV);

        float openAngle = doorAtMaxZ ? -90.0f : 90.0f;
        doorTargetAngle = doorOpen ? openAngle : 0.0f;
        doorAngle = doorTargetAngle;

                        
        exteriorDecor.clear();

        std::vector<BasicVertex> panelV, frameV, signV, accentV;

        float doorPlaneZ = (doorAtMaxZ ? size : -size);
        float faceSign = (doorAtMaxZ ? 1.0f : -1.0f);

        float panelZ = doorPlaneZ + faceSign * 0.10f;
        float frameCenterZ = doorPlaneZ + faceSign * 0.18f;

        float DW = this->doorWidth;
        float DH = this->doorHeight;
        float sideGap = 0.08f;
        float topY = 6.8f;

        glm::vec3 baseCol = { 0.10f, 0.10f, 0.12f };
        glm::vec3 accent = accentColor;

                addQuadLocal(panelV,
            { -size + 0.02f, 0.0f, panelZ },
            { -DW / 2 - sideGap, 0.0f, panelZ },
            { -DW / 2 - sideGap, topY, panelZ },
            { -size + 0.02f, topY, panelZ },
            baseCol);

        addQuadLocal(panelV,
            { DW / 2 + sideGap, 0.0f, panelZ },
            { size - 0.02f, 0.0f, panelZ },
            { size - 0.02f, topY, panelZ },
            { DW / 2 + sideGap, topY, panelZ },
            baseCol);

        addQuadLocal(panelV,
            { -DW / 2 - sideGap, DH, panelZ },
            { DW / 2 + sideGap, DH, panelZ },
            { DW / 2 + sideGap, topY, panelZ },
            { -DW / 2 - sideGap, topY, panelZ },
            baseCol * 1.05f);

                float frameThickness = 0.25f;
        float z1 = frameCenterZ - faceSign * 0.08f;
        float z2 = frameCenterZ + faceSign * 0.22f;

        addBoxLocal(frameV, { -DW / 2 - frameThickness, 0.0f, z1 }, { -DW / 2, DH, z2 }, accent * 0.85f);
        addBoxLocal(frameV, { DW / 2, 0.0f, z1 }, { DW / 2 + frameThickness, DH, z2 }, accent * 0.85f);
        addBoxLocal(frameV, { -DW / 2 - frameThickness, DH, z1 }, { DW / 2 + frameThickness, DH + frameThickness, z2 }, accent);

                float signH1 = topY + 0.10f;
        float signH2 = topY + 0.90f;
        float signPad = 1.2f;
        float signZ = frameCenterZ + faceSign * 0.03f;

        addQuadLocal(signV,
            { -DW / 2 - signPad, signH1, signZ },
            { DW / 2 + signPad, signH1, signZ },
            { DW / 2 + signPad, signH2, signZ },
            { -DW / 2 - signPad, signH2, signZ },
            accent * 0.55f);

                if (name == "Electric")
        {
            float w = 0.14f;
            float xEdge = size - 0.30f;
            addQuadLocal(accentV, { xEdge - w, 0.4f, panelZ + faceSign * 0.02f }, { xEdge, 0.4f, panelZ + faceSign * 0.02f },
                { xEdge, topY - 0.2f, panelZ + faceSign * 0.02f }, { xEdge - w, topY - 0.2f, panelZ + faceSign * 0.02f }, accent);
            addQuadLocal(accentV, { -xEdge, 0.4f, panelZ + faceSign * 0.02f }, { -xEdge + w, 0.4f, panelZ + faceSign * 0.02f },
                { -xEdge + w, topY - 0.2f, panelZ + faceSign * 0.02f }, { -xEdge, topY - 0.2f, panelZ + faceSign * 0.02f }, accent);
        }
        else if (name == "Sports")
        {
            for (int i = 0; i < 3; i++)
            {
                float x = -size + 1.8f + i * 1.1f;
                addQuadLocal(accentV,
                    { x, 0.8f, panelZ + faceSign * 0.02f },
                    { x + 0.35f, 0.8f, panelZ + faceSign * 0.02f },
                    { x + 2.1f, 5.8f, panelZ + faceSign * 0.02f },
                    { x + 1.7f, 5.8f, panelZ + faceSign * 0.02f },
                    accent * 0.9f);
            }
        }
        else if (name == "Family")
        {
            addQuadLocal(accentV,
                { -6.5f, topY + 0.95f, signZ },
                { 6.5f, topY + 0.95f, signZ },
                { 6.5f, topY + 1.10f, signZ },
                { -6.5f, topY + 1.10f, signZ },
                accent * 0.7f);
        }

        exteriorDecor.emplace_back(panelV);
        exteriorDecor.emplace_back(frameV);
        exteriorDecor.emplace_back(signV);
        exteriorDecor.emplace_back(accentV);

                        
        decorations.clear();

        auto addQuad = [&](std::vector<BasicVertex>& v,
            glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d,
            glm::vec3 col)
            {
                glm::vec3 normal = glm::normalize(glm::cross(b - a, c - a));
                v.push_back({ a, col, normal });
                v.push_back({ b, col, normal });
                v.push_back({ c, col, normal });
                v.push_back({ a, col, normal });
                v.push_back({ c, col, normal });
                v.push_back({ d, col, normal });
            };

        if (roomName == "Luxury")
        {
            std::vector<BasicVertex> rugV, goldV, chandelierV;
            float y = 0.02f;
            float rug = size * 0.75f;

            glm::vec3 rugCol = { 0.08f, 0.08f, 0.10f };
            glm::vec3 goldCol = { 0.9f, 0.75f, 0.25f };

            addQuad(rugV, addPos({ -rug, y, -rug }), addPos({ rug, y, -rug }),
                addPos({ rug, y,  rug }), addPos({ -rug, y,  rug }), rugCol);

            float bw = 0.35f;
            addQuad(goldV, addPos({ -rug, y + 0.001f, -rug }), addPos({ rug, y + 0.001f, -rug }),
                addPos({ rug, y + 0.001f, -rug + bw }), addPos({ -rug, y + 0.001f, -rug + bw }), goldCol);
            addQuad(goldV, addPos({ -rug, y + 0.001f, rug - bw }), addPos({ rug, y + 0.001f, rug - bw }),
                addPos({ rug, y + 0.001f, rug }), addPos({ -rug, y + 0.001f, rug }), goldCol);
            addQuad(goldV, addPos({ -rug, y + 0.001f, -rug }), addPos({ -rug + bw, y + 0.001f, -rug }),
                addPos({ -rug + bw, y + 0.001f, rug }), addPos({ -rug, y + 0.001f, rug }), goldCol);
            addQuad(goldV, addPos({ rug - bw, y + 0.001f, -rug }), addPos({ rug, y + 0.001f, -rug }),
                addPos({ rug, y + 0.001f, rug }), addPos({ rug - bw, y + 0.001f, rug }), goldCol);

                        float chandY = h - 1.5f;
            float chandR = 1.5f;
            glm::vec3 chandColor = goldCol * 0.8f;
            glm::vec3 chandGlow = { 1.0f, 0.95f, 0.8f };

            int segments = 8;
            for (int i = 0; i < segments; i++)
            {
                float angle1 = (2.0f * 3.14159f * i) / segments;
                float angle2 = (2.0f * 3.14159f * (i + 1)) / segments;

                glm::vec3 p1 = addPos({ 0, chandY, 0 });
                glm::vec3 p2 = addPos({ chandR * cos(angle1), chandY - 0.3f, chandR * sin(angle1) });
                glm::vec3 p3 = addPos({ chandR * cos(angle2), chandY - 0.3f, chandR * sin(angle2) });

                chandelierV.push_back({ p1, chandGlow, {0, -1, 0} });
                chandelierV.push_back({ p2, chandColor, glm::normalize(glm::vec3(cos(angle1), -0.5f, sin(angle1))) });
                chandelierV.push_back({ p3, chandColor, glm::normalize(glm::vec3(cos(angle2), -0.5f, sin(angle2))) });
            }

            decorations.emplace_back(rugV);
            decorations.emplace_back(goldV);
            decorations.emplace_back(chandelierV);
        }
        else if (roomName == "Electric")
        {
            std::vector<BasicVertex> neonV, hexV;
            float y = 0.03f;
            float lineW = 0.18f;
            glm::vec3 neon = { 0.2f, 0.9f, 1.0f };
            glm::vec3 neonBright = neon * 1.5f;

            float edge = size * 0.90f;

                        addQuad(neonV, addPos({ -edge, y, -edge }), addPos({ -edge + lineW, y, -edge }),
                addPos({ -edge + lineW, y, edge }), addPos({ -edge, y, edge }), neon);
            addQuad(neonV, addPos({ edge - lineW, y, -edge }), addPos({ edge, y, -edge }),
                addPos({ edge, y, edge }), addPos({ edge - lineW, y, edge }), neon);
            addQuad(neonV, addPos({ -edge, y, -edge }), addPos({ edge, y, -edge }),
                addPos({ edge, y, -edge + lineW }), addPos({ -edge, y, -edge + lineW }), neon);
            addQuad(neonV, addPos({ -edge, y, edge - lineW }), addPos({ edge, y, edge - lineW }),
                addPos({ edge, y, edge }), addPos({ -edge, y, edge }), neon);

                        float hexY = 0.015f;
            float hexR = 2.5f;
            int hexSeg = 6;
            for (int i = 0; i < hexSeg; i++)
            {
                float angle1 = (2.0f * 3.14159f * i) / hexSeg;
                float angle2 = (2.0f * 3.14159f * (i + 1)) / hexSeg;

                glm::vec3 p1 = addPos({ 0, hexY, 0 });
                glm::vec3 p2 = addPos({ hexR * cos(angle1), hexY, hexR * sin(angle1) });
                glm::vec3 p3 = addPos({ hexR * cos(angle2), hexY, hexR * sin(angle2) });

                hexV.push_back({ p1, neonBright, upNormal });
                hexV.push_back({ p2, neon * 0.5f, upNormal });
                hexV.push_back({ p3, neon * 0.5f, upNormal });
            }

            decorations.emplace_back(neonV);
            decorations.emplace_back(hexV);
        }
        else if (roomName == "Sports")
        {
            std::vector<BasicVertex> stripeV, bannerV, checkeredV;
            float y = 0.02f;
            glm::vec3 red = { 0.85f, 0.1f, 0.1f };
            glm::vec3 white = { 0.9f, 0.9f, 0.9f };
            glm::vec3 banner = { 0.08f, 0.08f, 0.08f };

                        float sW = 2.0f;
            addQuad(stripeV,
                addPos({ -sW, y,  size * 0.95f }),
                addPos({ sW, y,  size * 0.95f }),
                addPos({ sW, y, -size * 0.95f }),
                addPos({ -sW, y, -size * 0.95f }), red);

                        float checkY = 0.025f;
            float checkSize = 0.8f;
            float checkStart = isDoorAtMaxZ ? size * 0.7f : -size * 0.7f;

            for (int i = 0; i < 6; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    float cx = -2.5f + i * checkSize;
                    float cz = checkStart + (isDoorAtMaxZ ? -j : j) * checkSize;
                    glm::vec3 checkCol = ((i + j) % 2 == 0) ? white : glm::vec3(0.1f);

                    addQuad(checkeredV,
                        addPos({ cx, checkY, cz }),
                        addPos({ cx + checkSize, checkY, cz }),
                        addPos({ cx + checkSize, checkY, cz + (isDoorAtMaxZ ? -checkSize : checkSize) }),
                        addPos({ cx, checkY, cz + (isDoorAtMaxZ ? -checkSize : checkSize) }),
                        checkCol);
                }
            }

                        float by1 = 4.2f, by2 = 6.2f;
            addQuad(bannerV,
                addPos({ -size + 0.02f, by1, -4.0f }),
                addPos({ -size + 0.02f, by1,  4.0f }),
                addPos({ -size + 0.02f, by2,  4.0f }),
                addPos({ -size + 0.02f, by2, -4.0f }), banner);

            decorations.emplace_back(stripeV);
            decorations.emplace_back(bannerV);
            decorations.emplace_back(checkeredV);
        }
        else if (roomName == "Family")
        {
            std::vector<BasicVertex> rugV, sofaV, framesV, plantsV;
            float y = 0.02f;

            glm::vec3 rugCol = { 0.55f, 0.45f, 0.35f };
            glm::vec3 rugBorder = { 0.40f, 0.30f, 0.20f };
            glm::vec3 sofaCol = { 0.20f, 0.20f, 0.22f };
            glm::vec3 frameCol = { 0.85f, 0.85f, 0.88f };
            glm::vec3 picCol = { 0.20f, 0.45f, 0.60f };
            glm::vec3 plantGreen = { 0.2f, 0.5f, 0.2f };
            glm::vec3 potColor = { 0.4f, 0.25f, 0.15f };

                        float rug = size * 0.60f;
            float rugBorderW = 0.4f;

                        addQuadLocal(rugV,
                { -rug + rugBorderW, y, -rug + rugBorderW },
                { rug - rugBorderW, y, -rug + rugBorderW },
                { rug - rugBorderW, y, rug - rugBorderW },
                { -rug + rugBorderW, y, rug - rugBorderW },
                rugCol);

                        addQuadLocal(rugV, { -rug, y + 0.001f, -rug }, { rug, y + 0.001f, -rug },
                { rug, y + 0.001f, -rug + rugBorderW }, { -rug, y + 0.001f, -rug + rugBorderW }, rugBorder);
            addQuadLocal(rugV, { -rug, y + 0.001f, rug - rugBorderW }, { rug, y + 0.001f, rug - rugBorderW },
                { rug, y + 0.001f, rug }, { -rug, y + 0.001f, rug }, rugBorder);
            addQuadLocal(rugV, { -rug, y + 0.001f, -rug }, { -rug + rugBorderW, y + 0.001f, -rug },
                { -rug + rugBorderW, y + 0.001f, rug }, { -rug, y + 0.001f, rug }, rugBorder);
            addQuadLocal(rugV, { rug - rugBorderW, y + 0.001f, -rug }, { rug, y + 0.001f, -rug },
                { rug, y + 0.001f, rug }, { rug - rugBorderW, y + 0.001f, rug }, rugBorder);

                        float sx1 = -size + 0.8f;
            float sx2 = -size + 2.8f;
            float sz1 = -10.5f;
            float sz2 = -4.5f;
            addBoxLocal(sofaV, { sx1, 0.0f, sz1 }, { sx2, 1.0f, sz2 }, sofaCol);

                        addBoxLocal(sofaV, { sx1, 1.0f, sz1 }, { sx1 + 0.3f, 1.8f, sz2 }, sofaCol * 0.9f);

                        float wx = size - 0.05f;
            float by1 = 3.2f, by2 = 4.2f;

            for (int i = 0; i < 3; i++)
            {
                float wz1 = -4.0f + i * 3.0f;
                float wz2 = wz1 + 2.0f;

                addQuadLocal(framesV,
                    { wx, by1, wz1 }, { wx, by1, wz2 }, { wx, by2, wz2 }, { wx, by2, wz1 },
                    frameCol);

                glm::vec3 artColor = picCol * (0.8f + 0.1f * i);
                addQuadLocal(framesV,
                    { wx - 0.01f, by1 + 0.15f, wz1 + 0.15f },
                    { wx - 0.01f, by1 + 0.15f, wz2 - 0.15f },
                    { wx - 0.01f, by2 - 0.15f, wz2 - 0.15f },
                    { wx - 0.01f, by2 - 0.15f, wz1 + 0.15f },
                    artColor);
            }

                        float plantX = size - 2.0f;
            float plantZ = isDoorAtMaxZ ? -size + 2.0f : size - 2.0f;

                        addBoxLocal(plantsV, { plantX - 0.4f, 0, plantZ - 0.4f }, { plantX + 0.4f, 0.6f, plantZ + 0.4f }, potColor);

                        int leafSegs = 6;
            for (int i = 0; i < leafSegs; i++)
            {
                float angle1 = (2.0f * 3.14159f * i) / leafSegs;
                float angle2 = (2.0f * 3.14159f * (i + 1)) / leafSegs;
                float leafR = 0.6f;

                plantsV.push_back({ addPos({ plantX, 1.8f, plantZ }), plantGreen * 1.2f, {0, 1, 0} });
                plantsV.push_back({ addPos({ plantX + leafR * cos(angle1), 0.6f, plantZ + leafR * sin(angle1) }), plantGreen,
                    glm::normalize(glm::vec3(cos(angle1), 0.3f, sin(angle1))) });
                plantsV.push_back({ addPos({ plantX + leafR * cos(angle2), 0.6f, plantZ + leafR * sin(angle2) }), plantGreen,
                    glm::normalize(glm::vec3(cos(angle2), 0.3f, sin(angle2))) });
            }

            decorations.emplace_back(rugV);
            decorations.emplace_back(sofaV);
            decorations.emplace_back(framesV);
            decorations.emplace_back(plantsV);
        }
    }

            
    void Room::draw(const glm::mat4& viewProj)
    {
        floor.render(glm::mat4(1.0f), viewProj);
        walls.render(glm::mat4(1.0f), viewProj);

        for (auto& ext : exteriorDecor)
            ext.render(glm::mat4(1.0f), viewProj);

        for (auto& podium : podiums)
            podium.render(glm::mat4(1.0f), viewProj);

        for (auto& deco : decorations)
            deco.render(glm::mat4(1.0f), viewProj);

                float doorZLocal = doorAtMaxZ ? size : -size;
        float faceSign = doorAtMaxZ ? 1.0f : -1.0f;
        glm::vec3 hingeWorld = centerOffset + glm::vec3(-this->doorWidth / 2.0f, 0.0f, doorZLocal + faceSign * 0.03f);

        glm::mat4 doorModel(1.0f);
        doorModel = glm::translate(doorModel, hingeWorld);
        doorModel = glm::rotate(doorModel, glm::radians(doorAngle), glm::vec3(0, 1, 0));

        doorPanel.render(doorModel, viewProj);

        ceilingLight.render(glm::mat4(1.0f), viewProj);

        for (auto& car : cars)
            car->render(viewProj);
    }

    void Room::addExistingCarToPodium(int podiumIndex, Example::Car* car)
    {
        if (podiumIndex < 0 || podiumIndex >= (int)podiumBounds.size())
            return;

        const auto& podium = podiumBounds[podiumIndex];

        float podiumTopY = 0.25f;
        glm::vec3 carPos = { podium.xCenter, podiumTopY - car->getGroundClearance(), podium.zCenter };

        float carRotation = doorAtMaxZ ? 180.0f : 0.0f;
        car->setPosition(carPos);

        cars.push_back(std::unique_ptr<Car>(car));
    }

    void Room::addCarToPodium(int podiumIndex, glm::vec3 carColor, glm::vec3 interiorColor)
    {
        if (podiumIndex < 0 || podiumIndex >= (int)podiumBounds.size())
            return;

        const auto& podium = podiumBounds[podiumIndex];
        float podiumTopY = 0.25f;

        auto car = std::make_unique<Car>();
        glm::vec3 carPos = { podium.xCenter, 0.0f, podium.zCenter };
        float carRotation = doorAtMaxZ ? 180.0f : 0.0f;
        car->create(carPos, carRotation, carColor, interiorColor);
        carPos.y = podiumTopY - car->getGroundClearance();
        car->setPosition(carPos);

        cars.push_back(std::move(car));
    }

    Car* Room::getCarNearPlayer(glm::vec3 playerPos)
    {
        for (auto& car : cars)
        {
            if (car->isNearDriverDoor(playerPos))
                return car.get();
        }
        return nullptr;
    }

    void Room::toggleDoor()
    {
        doorOpen = !doorOpen;
        float openAngle = doorAtMaxZ ? -90.0f : 90.0f;
        doorTargetAngle = doorOpen ? openAngle : 0.0f;
    }

    void Room::update(float dt)
    {
        float speedDeg = 180.0f;
        float diff = doorTargetAngle - doorAngle;

        if (std::abs(diff) <= 0.5f)
            doorAngle = doorTargetAngle;
        else
        {
            float step = speedDeg * dt;
            if (diff > 0) doorAngle += std::min(step, diff);
            else          doorAngle -= std::min(step, -diff);
        }
    }

    bool Room::isNearDoor(const glm::vec3& playerPos) const
    {
        float doorZLocal = doorAtMaxZ ? size : -size;
        glm::vec3 doorCenter = centerOffset + glm::vec3(0.0f, 0.0f, doorZLocal);

        float dx = std::abs(playerPos.x - doorCenter.x);
        float dz = std::abs(playerPos.z - doorCenter.z);

        return (dx < 4.0f && dz < 2.0f);
    }
}