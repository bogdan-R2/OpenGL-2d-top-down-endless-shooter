#include "lab_m1/tema/tema.h"

#include <vector>
#include <iostream>

#include "lab_m1/tema/transform2D.h"
#include "lab_m1/tema/object2D.h"

using namespace std;
using namespace m1;


tema::tema()
{
}


tema::~tema()
{
}


void tema::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    modelMatrix = glm::mat3(1);
    modelMatrixPlayer = glm::mat3(1);

    glm::vec3 corner = glm::vec3(0, 0, 0);

    playerX = 0;
    playerY = 0;
    enemyX = 0;
    enemyY = 0;

    translateObstacle1X = 150;
    translateObstacle1Y = 300;
    translateObstacle2X = 1000;
    translateObstacle2Y = 1300;
    translateObstacle3X = 1400;
    translateObstacle3Y = 200;
    translateObstacle4X = 200;
    translateObstacle4Y = 1600;
    translateObstacle5X = 800;
    translateObstacle5Y = 600;
    translateObstacle6X = 200;
    translateObstacle6Y = 1300;
    scaleObstacle1X = 3;
    scaleObstacle1Y = 4;
    scaleObstacle2X = 7;
    scaleObstacle2Y = 4;
    scaleObstacle3X = 5;
    scaleObstacle3Y = 5;
    scaleObstacle4X = 5;
    scaleObstacle4Y = 3;
    scaleObstacle5X = 8;
    scaleObstacle5Y = 5;
    scaleObstacle6X = 3;
    scaleObstacle6Y = 5;

    cursorAngle = 0;
    playerAngle = 0;

    bulletSpeed = 0;
    shotInterval = 0;
    playerScore = 0;
    enemyScore = 0;
    health = 100;

    {
        Mesh* map = object2D::CreateSquare("map", corner, 2000, glm::vec3(0, 0.8f, 0), true);
        AddMeshToList(map);
    }
    
    {
        Mesh* obstacle = object2D::CreateSquare("obstacle", corner, 100, glm::vec3(0.2f, 0.01f, 0), true);
        AddMeshToList(obstacle);
    }
    
    {
        Mesh* healthbar = object2D::CreateSquare("healthbar", corner, 100, glm::vec3(1, 0, 0), true);
        AddMeshToList(healthbar);
    }
    
    {
        Mesh* wireframe = object2D::CreateSquare("wireframe", corner, 100, glm::vec3(0, 0, 0));
        AddMeshToList(wireframe);
    }
    
    {
        Mesh* player = object2D::CreateSquare("player", corner, 100, glm::vec3(0, 1, 1), true);
        AddMeshToList(player);
    }
    
    {
        Mesh* eye = object2D::CreateSquare("eye", corner, 100, glm::vec3(0, 0, 0), true);
        AddMeshToList(eye);
    }
    
    {
        Mesh* bullet = object2D::CreateSquare("bullet", corner, 20, glm::vec3(0, 0, 0), true);
        AddMeshToList(bullet);
    }
   
    {
        Mesh* enemy = object2D::CreateSquare("enemy", corner, 100, glm::vec3(0.8f, 0.4f, 0), true);
        AddMeshToList(enemy);
    }
    
    {
        Mesh* leg = object2D::CreateSquare("leg", corner, 100, glm::vec3(0.4f, 0.2f, 0), true);
        AddMeshToList(leg);
    }
    
    glLineWidth(2);
}


void tema::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.52f, 0.8f, 0.92f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void tema::Healthbar(glm::mat3 modelMatrix)
{
    modelMatrix = transform2D::Translate(playerX - 140, playerY + 350);
    modelMatrix *= transform2D::Scale(4, 0.4f);
    RenderMesh2D(meshes["wireframe"], shaders["VertexColor"], modelMatrix);
    modelMatrix = transform2D::Translate(playerX - 140, playerY + 350);
    modelMatrix *= transform2D::Scale(4 * health / 100, 0.4f);
    RenderMesh2D(meshes["healthbar"], shaders["VertexColor"], modelMatrix);
}


void tema::Map(glm::mat3 modelMatrix) 
{
    modelMatrix = transform2D::Translate(0, 0);
    RenderMesh2D(meshes["map"], shaders["VertexColor"], modelMatrix);
}


void tema::Obstacle(glm::mat3 modelMatrix, float translateX, float translateY, float scaleX, float scaleY)
{
    modelMatrix = transform2D::Translate(translateX, translateY);
    modelMatrix *= transform2D::Scale(scaleX, scaleY);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);
}


bool tema::BulletCollision(glm::mat3 modelMatrix, float translateX, float translateY)
{
    if (modelMatrix[2][0] < translateX + 100 &&
        modelMatrix[2][1] < translateY + 100 &&
        modelMatrix[2][0] > translateX &&
        modelMatrix[2][1] > translateY + 20)
        return true;
    else return false;
}


bool tema::PlayerCollision(float playerX, float playerY, float enemyX, float enemyY)
{
    if (playerX < enemyX + 100 &&
        playerY < enemyY + 100 &&
        playerX + 100 > enemyX &&
        playerY + 100 > enemyY)
        return true;
    else return false;
}


bool tema::BulletObstacleCollision(glm::mat3 modelMatrix, float translateX, float translateY, float scaleX, float scaleY)
{
    if (translateX < modelMatrix[2][0] + 20 &&
        translateX + 100 * scaleX > modelMatrix[2][0] &&
        translateY < modelMatrix[2][1] + 20 &&
        100 * scaleY + translateY > modelMatrix[2][1])
        return true;
    else return false;
}


bool tema::PlayerObstacleCollisionW(float playerX, float playerY, float obstacleX, float obstacleY, float scaleX, float scaleY, float deltaTime)
{
    if (obstacleX < playerX + 100 &&
        obstacleX + 100 * scaleX > playerX &&
        obstacleY < playerY + 100 + deltaTime * 500 &&
        100 * scaleY + obstacleY > playerY)
        return true;
    else return false;
}


bool tema::PlayerObstacleCollisionS(float playerX, float playerY, float obstacleX, float obstacleY, float scaleX, float scaleY, float deltaTime)
{
    if (obstacleX < playerX + 100 &&
        obstacleX + 100 * scaleX > playerX &&
        obstacleY < playerY + 100 &&
        100 * scaleY + obstacleY + deltaTime * 500 > playerY)
        return true;
    else return false;
}


bool tema::PlayerObstacleCollisionA(float playerX, float playerY, float obstacleX, float obstacleY, float scaleX, float scaleY, float deltaTime)
{
    if (obstacleX < playerX + 100 &&
        obstacleX + 100 * scaleX + deltaTime * 500 > playerX &&
        obstacleY < playerY + 100 &&
        100 * scaleY + obstacleY > playerY)
        return true;
    else return false;
}


bool tema::PlayerObstacleCollisionD(float playerX, float playerY, float obstacleX, float obstacleY, float scaleX, float scaleY, float deltaTime)
{
    if (obstacleX < playerX + 100 + deltaTime * 500 &&
        obstacleX + 100 * scaleX > playerX &&
        obstacleY < playerY + 100 &&
        100 * scaleY + obstacleY > playerY)
        return true;
    else return false;
}


void tema::Update(float deltaTimeSeconds)
{
    // Translate camera in center
    glm::vec3 camera = glm::vec3(playerX - window->GetResolution()[0] / 2 + 100 / 2,
        playerY - window->GetResolution()[1] / 2 + 100 / 2, 50);
    GetSceneCamera()->SetPosition(camera);

    // Cursor angle
    cursorAngle = atan2(window->GetCursorPosition()[1] - window->GetResolution()[1] / 2,
        window->GetCursorPosition()[0] - window->GetResolution()[0] / 2);
  
    // Render the Healthbar
    Healthbar(modelMatrix);

    // Render the player
    {
        // Rotation after cursor angle
        modelMatrixPlayer = transform2D::Translate(playerX + 100 / 2, playerY + 100 / 2);
        modelMatrixPlayer *= transform2D::Rotate(-cursorAngle - 3.14f / 2);
        modelMatrixPlayer *= transform2D::Translate(-50, -50);

        // Eyes
        modelMatrixEye = modelMatrixPlayer * transform2D::Translate(60, 60);
        modelMatrixEye *= transform2D::Scale(0.2f, 0.2f);
        RenderMesh2D(meshes["eye"], shaders["VertexColor"], modelMatrixEye);
        modelMatrixEye = modelMatrixEye * transform2D::Translate(-200, 0);
        RenderMesh2D(meshes["eye"], shaders["VertexColor"], modelMatrixEye);

        // Player
        RenderMesh2D(meshes["player"], shaders["VertexColor"], modelMatrixPlayer);
    }

    // Render enemies 
    {
        // Bullet-enemy collision
        if (BulletCollision(modelMatrixBullet, enemyX, enemyY)) 
        {
            // Increase player score if enemy is killed
            playerScore++;

            // Print the score
            printf("SCORE: %d - %d\n", playerScore, enemyScore);

            // Enemy-map collision
            while (!(enemyX < modelMatrixPlayer[2][0] - 500 || enemyX > modelMatrixPlayer[2][0] + 500))
            {
                // Respawn the enemies at random positions
                enemyX = rand() % 2000;
            }
            // Enemy-map collision
            while (!(enemyY < modelMatrixPlayer[2][1] - 500 || enemyY > modelMatrixPlayer[2][1] + 500)) 
            {
                // Respawn the enemies at random positions
                enemyY = rand() % 2000;
            }
        }
        
        // Enemy-player collision
        if (PlayerCollision(playerX, playerY, enemyX, enemyY))
        {
            // Increase enemy score if player is hit
            enemyScore++;

            // Print the score
            printf("SCORE: %d - %d\n", playerScore, enemyScore);

            // Decrease health if player is hit and health is positive
            if (health > 0)
            {
                health -= 10;
            }
            // The game ends when health is 0
            else
            {
                printf("GAME OVER\n");
                printf("FINAL SCORE : %d - %d\n", playerScore, enemyScore);
                window->Close();
            }

            // Enemy-player collision
            while (!(enemyX < modelMatrixPlayer[2][0] - 500 || enemyX > modelMatrixPlayer[2][0] + 500))
            {
                // Respawn the enemies at random positions
                enemyX = rand() % 2000;
            }
            // Enemy-player collision
            while (!(enemyY < modelMatrixPlayer[2][1] - 500 || enemyY > modelMatrixPlayer[2][1] + 500)) 
            {
                // Respawn the enemies at random positions
                enemyY = rand() % 2000;
            }
        }

        // Enemy rotates after player
        playerAngle = atan2((modelMatrixPlayer * transform2D::Translate(50, 50))[2][0] 
                                            - (modelMatrixEnemy * transform2D::Translate(50, 50))[2][0],
                            (modelMatrixPlayer * transform2D::Translate(50, 50))[2][1] 
                                            - (modelMatrixEnemy * transform2D::Translate(50, 50))[2][1]);       
        norm = sqrt((enemyX - playerX) * (enemyX - playerX) + (enemyY - playerY) * (enemyY - playerY));

        // Enemy follows the player
        enemyX += (playerX - enemyX) / norm * deltaTimeSeconds * 500;
        enemyY += (playerY - enemyY) / norm * deltaTimeSeconds * 500;
        modelMatrixEnemy = transform2D::Translate(enemyX + 50, enemyY + 50);
        modelMatrixEnemy *= transform2D::Rotate(-playerAngle);
        modelMatrixEnemy *= transform2D::Translate(-50, -50);

        // Render enemy eyes
        modelMatrixEye = modelMatrixEnemy * transform2D::Translate(60, 60);
        modelMatrixEye *= transform2D::Scale(0.2f, 0.2f);
        RenderMesh2D(meshes["eye"], shaders["VertexColor"], modelMatrixEye);
        modelMatrixEye = modelMatrixEye * transform2D::Translate(-200, 0);
        RenderMesh2D(meshes["eye"], shaders["VertexColor"], modelMatrixEye);

        // Render enemy legs
        modelMatrixEnemyLeg = modelMatrixEnemy * transform2D::Translate(0, 100);
        modelMatrixEnemyLeg *= transform2D::Scale(0.3f, 0.3f);
        RenderMesh2D(meshes["leg"], shaders["VertexColor"], modelMatrixEnemyLeg);
        modelMatrixEnemyLeg = modelMatrixEnemyLeg * transform2D::Translate(233.33f, 0);
        RenderMesh2D(meshes["leg"], shaders["VertexColor"], modelMatrixEnemyLeg);
        modelMatrixEnemyLeg = modelMatrixEnemyLeg * transform2D::Translate(-233.33f, -420);
        RenderMesh2D(meshes["leg"], shaders["VertexColor"], modelMatrixEnemyLeg);
        modelMatrixEnemyLeg = modelMatrixEnemyLeg * transform2D::Translate(233.33f, 0);
        RenderMesh2D(meshes["leg"], shaders["VertexColor"], modelMatrixEnemyLeg);

        // Render the enemy
        RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrixEnemy);
    }

    // Bullet
    {
        // Increase time interval between shots (must be > 0.5 sec)
        shotInterval += deltaTimeSeconds;

        // Bullet-map and bullet-obstacle collision
        if ((modelMatrixBullet[2][0] < 2000 &&
            modelMatrixBullet[2][1] < 2000 &&
            modelMatrixBullet[2][0] > 0 &&
            modelMatrixBullet[2][1] > 20)   
            &&
            (!BulletObstacleCollision(modelMatrixBullet, translateObstacle1X, translateObstacle1Y, scaleObstacle1X, scaleObstacle1Y))
            &&
            (!BulletObstacleCollision(modelMatrixBullet, translateObstacle2X, translateObstacle2Y, scaleObstacle2X, scaleObstacle2Y))
            &&
            (!BulletObstacleCollision(modelMatrixBullet, translateObstacle3X, translateObstacle3Y, scaleObstacle3X, scaleObstacle3Y))
            &&
            (!BulletObstacleCollision(modelMatrixBullet, translateObstacle4X, translateObstacle4Y, scaleObstacle4X, scaleObstacle4Y))
            &&
            (!BulletObstacleCollision(modelMatrixBullet, translateObstacle5X, translateObstacle5Y, scaleObstacle5X, scaleObstacle5Y))
            &&
            (!BulletObstacleCollision(modelMatrixBullet, translateObstacle6X, translateObstacle6Y, scaleObstacle6X, scaleObstacle6Y)))
        {
            // Render the bullet
            bulletSpeed += 200 * deltaTimeSeconds;
            modelMatrixBullet *= transform2D::Translate(0, bulletSpeed);
            RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrixBullet);
        }
        else {
            modelMatrixBullet *= transform2D::Translate(3000, 3000);
        }
    }

    // Render the obstacles
    modelMatrix = glm::mat3(1);
    Obstacle(modelMatrix, translateObstacle1X, translateObstacle1Y, scaleObstacle1X, scaleObstacle1Y);
    Obstacle(modelMatrix, translateObstacle2X, translateObstacle2Y, scaleObstacle2X, scaleObstacle2Y);
    Obstacle(modelMatrix, translateObstacle3X, translateObstacle3Y, scaleObstacle3X, scaleObstacle3Y);
    Obstacle(modelMatrix, translateObstacle4X, translateObstacle4Y, scaleObstacle4X, scaleObstacle4Y);
    Obstacle(modelMatrix, translateObstacle5X, translateObstacle5Y, scaleObstacle5X, scaleObstacle5Y);
    Obstacle(modelMatrix, translateObstacle6X, translateObstacle6Y, scaleObstacle6X, scaleObstacle6Y);
   
    // Render the map
    modelMatrix = glm::mat3(1);
    Map(modelMatrix);
}


void tema::FrameEnd()
{
}


void tema::OnInputUpdate(float deltaTime, int mods)
{
    // Move with W
    if (window->KeyHold(GLFW_KEY_W) && (playerY < 2000 - 100)) {
        // Player-obstacle collision
        if (!PlayerObstacleCollisionW(playerX, playerY, translateObstacle1X, translateObstacle1Y, scaleObstacle1X, scaleObstacle1Y, deltaTime)
            &&
            !PlayerObstacleCollisionW(playerX, playerY, translateObstacle2X, translateObstacle2Y, scaleObstacle2X, scaleObstacle2Y, deltaTime)
            &&
            !PlayerObstacleCollisionW(playerX, playerY, translateObstacle3X, translateObstacle3Y, scaleObstacle3X, scaleObstacle3Y, deltaTime)
            &&
            !PlayerObstacleCollisionW(playerX, playerY, translateObstacle4X, translateObstacle4Y, scaleObstacle4X, scaleObstacle4Y, deltaTime)
            &&
            !PlayerObstacleCollisionW(playerX, playerY, translateObstacle5X, translateObstacle5Y, scaleObstacle5X, scaleObstacle5Y, deltaTime)
            &&
            !PlayerObstacleCollisionW(playerX, playerY, translateObstacle6X, translateObstacle6Y, scaleObstacle6X, scaleObstacle6Y, deltaTime))
            playerY += deltaTime * 500;
    }
    // Move with S
    if (window->KeyHold(GLFW_KEY_S) && playerY > 0) {
        // Player-obstacle collision
        if (!PlayerObstacleCollisionS(playerX, playerY, translateObstacle1X, translateObstacle1Y, scaleObstacle1X, scaleObstacle1Y, deltaTime)
            &&
            !PlayerObstacleCollisionS(playerX, playerY, translateObstacle2X, translateObstacle2Y, scaleObstacle2X, scaleObstacle2Y, deltaTime)
            &&
            !PlayerObstacleCollisionS(playerX, playerY, translateObstacle3X, translateObstacle3Y, scaleObstacle3X, scaleObstacle3Y, deltaTime)
            &&
            !PlayerObstacleCollisionS(playerX, playerY, translateObstacle4X, translateObstacle4Y, scaleObstacle4X, scaleObstacle4Y, deltaTime)
            &&
            !PlayerObstacleCollisionS(playerX, playerY, translateObstacle5X, translateObstacle5Y, scaleObstacle5X, scaleObstacle5Y, deltaTime)
            &&
            !PlayerObstacleCollisionS(playerX, playerY, translateObstacle6X, translateObstacle6Y, scaleObstacle6X, scaleObstacle6Y, deltaTime))
            
            playerY -= deltaTime * 500;
    }
    // Move with A
    if (window->KeyHold(GLFW_KEY_A) && playerX > 0) {
        // Player-obstacle collision
        if (!PlayerObstacleCollisionA(playerX, playerY, translateObstacle1X, translateObstacle1Y, scaleObstacle1X, scaleObstacle1Y, deltaTime)
            &&
            !PlayerObstacleCollisionA(playerX, playerY, translateObstacle2X, translateObstacle2Y, scaleObstacle2X, scaleObstacle2Y, deltaTime)
            &&
            !PlayerObstacleCollisionA(playerX, playerY, translateObstacle3X, translateObstacle3Y, scaleObstacle3X, scaleObstacle3Y, deltaTime)
            &&
            !PlayerObstacleCollisionA(playerX, playerY, translateObstacle4X, translateObstacle4Y, scaleObstacle4X, scaleObstacle4Y, deltaTime)
            &&
            !PlayerObstacleCollisionA(playerX, playerY, translateObstacle5X, translateObstacle5Y, scaleObstacle5X, scaleObstacle5Y, deltaTime)
            &&
            !PlayerObstacleCollisionA(playerX, playerY, translateObstacle6X, translateObstacle6Y, scaleObstacle6X, scaleObstacle6Y, deltaTime))
            
            playerX -= deltaTime * 500;
    }
    // Move with D 
    if (window->KeyHold(GLFW_KEY_D) && playerX < 2000 - 100) {
        // Player-obstacle collision
        if (!PlayerObstacleCollisionD(playerX, playerY, translateObstacle1X, translateObstacle1Y, scaleObstacle1X, scaleObstacle1Y, deltaTime)
            &&
            !PlayerObstacleCollisionD(playerX, playerY, translateObstacle2X, translateObstacle2Y, scaleObstacle2X, scaleObstacle2Y, deltaTime)
            &&
            !PlayerObstacleCollisionD(playerX, playerY, translateObstacle3X, translateObstacle3Y, scaleObstacle3X, scaleObstacle3Y, deltaTime)
            &&
            !PlayerObstacleCollisionD(playerX, playerY, translateObstacle4X, translateObstacle4Y, scaleObstacle4X, scaleObstacle4Y, deltaTime)
            &&
            !PlayerObstacleCollisionD(playerX, playerY, translateObstacle5X, translateObstacle5Y, scaleObstacle5X, scaleObstacle5Y, deltaTime)
            &&
            !PlayerObstacleCollisionD(playerX, playerY, translateObstacle6X, translateObstacle6Y, scaleObstacle6X, scaleObstacle6Y, deltaTime))
            
            playerX += deltaTime * 500;
    }
}


void tema::OnKeyPress(int key, int mods)
{
}


void tema::OnKeyRelease(int key, int mods)
{
}


void tema::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}


void tema::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Shoot the bullet if left mouse button is pressed
    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) && shotInterval > 0.5f) {
        shotInterval = 0; 
        bulletSpeed = 0;
        modelMatrixBullet = modelMatrixPlayer;
        modelMatrixBullet *= transform2D::Translate(50, 50);
    }
}


void tema::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void tema::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void tema::OnWindowResize(int width, int height)
{
}
