#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class tema : public gfxc::SimpleScene
    {
     public:
        tema();
        ~tema();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void Healthbar(glm::mat3 modelMatrix);
        void Map(glm::mat3 modelMatrix);
        void Obstacle(glm::mat3 modelMatrix, float translateX, float translateY, float scaleX, float scaleY);

        bool BulletCollision(glm::mat3 modelMatrix, float translateX, float translateY);
        bool PlayerCollision(float playerX, float playerY, float enemyX, float enemyY);
        bool BulletObstacleCollision(glm::mat3 modelMatrix, float translateX, float translateY, float scaleX, float scaleY);
        bool PlayerObstacleCollisionW(float playerX, float playerY, float obstacleX , float obstacleY, float scaleX, float scaleY, float deltaTime);
        bool PlayerObstacleCollisionS(float playerX, float playerY, float obstacleX, float obstacleY, float scaleX, float scaleY, float deltaTime);
        bool PlayerObstacleCollisionA(float playerX, float playerY, float obstacleX, float obstacleY, float scaleX, float scaleY, float deltaTime);
        bool PlayerObstacleCollisionD(float playerX, float playerY, float obstacleX, float obstacleY, float scaleX, float scaleY, float deltaTime);

     protected:
         glm::mat3 modelMatrix;
         glm::mat3 modelMatrixPlayer;
         glm::mat3 modelMatrixEye;
         glm::mat3 modelMatrixEnemy;
         glm::mat3 modelMatrixEnemyLeg;
         glm::mat3 modelMatrixBullet;
         glm::mat3 modelMatrixObstacle;

         float playerX, playerY;
         float enemyX, enemyY;
         float translateObstacle1X, translateObstacle1Y;
         float translateObstacle2X, translateObstacle2Y;
         float translateObstacle3X, translateObstacle3Y;
         float translateObstacle4X, translateObstacle4Y;
         float translateObstacle5X, translateObstacle5Y;
         float translateObstacle6X, translateObstacle6Y;
         float scaleObstacle1X, scaleObstacle1Y;
         float scaleObstacle2X, scaleObstacle2Y;
         float scaleObstacle3X, scaleObstacle3Y;
         float scaleObstacle4X, scaleObstacle4Y;
         float scaleObstacle5X, scaleObstacle5Y;
         float scaleObstacle6X, scaleObstacle6Y;
         float cursorAngle;
         float playerAngle;
         float bulletSpeed;
         float shotInterval;
         float health;
         float norm;

         int playerScore;
         int enemyScore;
    };
}   // namespace m1
