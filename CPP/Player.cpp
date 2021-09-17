//
//  Player.cpp
//  Game
//
//  Created by nltbao on 08/09/2021.
//

#include "Player.hpp"
#include "Texture.hpp"
#include "Game.hpp"
#include "Sprite.hpp"

std::vector<Bullet> Player::g_vBullet_bucket;

Player::Player(const char* file_name, const int PLAYER_X, const int PLAYER_Y, const int PLAYER_WIDTH, const int PLAYER_HEIGHT)
: PLAYER_WIDTH(PLAYER_WIDTH), PLAYER_HEIGHT(PLAYER_HEIGHT)
{
//    b_pPlayerTexture = Texture::LoadTexture(file_name);
    g_playerRect = { PLAYER_X, PLAYER_Y, PLAYER_WIDTH, PLAYER_HEIGHT };
    
//    sprite.emplace_back(Texture::LoadTexture("assets/imgs/player/Idle__000.png"));
//    sprite.emplace_back(Texture::LoadTexture("assets/imgs/player/Idle__001.png"));
//    sprite.emplace_back(Texture::LoadTexture("assets/imgs/player/Idle__002.png"));
//    sprite.emplace_back(Texture::LoadTexture("assets/imgs/player/Idle__003.png"));
//    sprite.emplace_back(Texture::LoadTexture("assets/imgs/player/Idle__004.png"));
//    sprite.emplace_back(Texture::LoadTexture("assets/imgs/player/Idle__005.png"));
//    sprite.emplace_back(Texture::LoadTexture("assets/imgs/player/Idle__006.png"));
//    sprite.emplace_back(Texture::LoadTexture("assets/imgs/player/Idle__007.png"));
//    sprite.emplace_back(Texture::LoadTexture("assets/imgs/player/Idle__008.png"));
//    sprite.emplace_back(Texture::LoadTexture("assets/imgs/player/Idle__009.png"));
    
    sprite.emplace_back(Texture::LoadTexture("assets/imgs/player/Dead__000.png"));
    sprite.emplace_back(Texture::LoadTexture("assets/imgs/player/Dead__001.png"));
    sprite.emplace_back(Texture::LoadTexture("assets/imgs/player/Dead__002.png"));
    sprite.emplace_back(Texture::LoadTexture("assets/imgs/player/Dead__003.png"));
    sprite.emplace_back(Texture::LoadTexture("assets/imgs/player/Dead__004.png"));
    sprite.emplace_back(Texture::LoadTexture("assets/imgs/player/Dead__005.png"));
    sprite.emplace_back(Texture::LoadTexture("assets/imgs/player/Dead__006.png"));
    sprite.emplace_back(Texture::LoadTexture("assets/imgs/player/Dead__007.png"));
    sprite.emplace_back(Texture::LoadTexture("assets/imgs/player/Dead__008.png"));
    sprite.emplace_back(Texture::LoadTexture("assets/imgs/player/Dead__009.png"));
    
}

Player::~Player()
{
    g_vBullet_bucket.clear();
}

void Player::update()
{
    g_playerRect.x += velocityX;
    velocityY = 0;
    
    // * Jump
    Player::on_jump();
    Player::on_fall();
        
    // * Boundaries X
    if(
       (g_playerRect.x < 0) ||
       (g_playerRect.x + PLAYER_WIDTH > Game::SCREEN_WIDTH) ||
       this->bCollision(g_playerRect.x, g_playerRect.y)
       )
    {
       g_playerRect.x -= velocityX;
    }

    // * Boundaries Y
    if(
       (g_playerRect.y < 0) ||
       (g_playerRect.y + PLAYER_HEIGHT > Game::SCREEN_HEIGHT) ||
       (g_playerRect.y + PLAYER_HEIGHT > Game::GROUND_HEIGHT))
    {
       g_playerRect.y -= velocityY;
    }

}

void Player::draw()
{
//    this->test_sprite();
//    Texture::DrawTexture(b_pPlayerTexture, NULL, &g_playerRect);
//    b_pPlayerTexture = Sprite::drawSprite(1, b_pPlayerTexture, NULL, &g_playerRect, 5);
//    Sprite::drawSprite(1, b_pPlayerTexture, NULL, &g_playerRect, 5);

//    b_pPlayerTexture = Sprite::getSprite(1)[0];
//    b_pPlayerTexture = Texture::LoadTexture("assets/imgs/player/Dead__000.png");
//    std::cout << Sprite::getSprite(1).size() << std::endl;
}

std::pair<int, int> bao;
Uint32 Player::test_sprite()
{
    bao.second++;
    if(bao.second == 2){
        test_idx++;
        b_pPlayerTexture = sprite[test_idx];
        if(test_idx >= sprite.size() - 1){
            test_idx= 0;
        }
        bao.second = 0;
    }
   
    
    return  0;
}

void Player::move_up(const bool bRevert)
{
    velocityY -= G_MOVE_SPEED;
}

void Player::move_down(const bool bRevert)
{
    velocityY += G_MOVE_SPEED;
}

void Player::move_left(const bool bRevert)
{
    velocityX -= G_MOVE_SPEED;
    if(!bRevert){
        g_lastDirection = DIRECTION.LEFT;
    }
}

void Player::move_right(const bool bRevert)
{
    velocityX += G_MOVE_SPEED;
    if(!bRevert){
        g_lastDirection = DIRECTION.RIGHT;
    }
}

void Player::on_start_jump()
{
    if(jump_status == 0){
        jump_status = 1;
        velocityY = 0;
        jumpStep = G_JUMP_HEIGHT;
    }
}

void Player::on_jump()
{
    if(jump_status == 1){
        // * Jumping
        velocityY += -abs(jumpStep);
        g_playerRect.y += velocityY;
        jumpStep += G_JUMP_SPEED;
        if(jumpStep > 0) {
            jump_status = 2;
        }
    }
}

void Player::on_fall()
{
    int adjustX = g_playerRect.x;
    int adjustY = g_playerRect.y;
    
    if(hold_right) {
        adjustX -= 5;
    } else if(hold_left){
        adjustX += 5;
    }
    
    if(hold_up) {
        adjustY += 5;
    } else if(hold_down){
        adjustY -= 5;
    }
    
    if(jump_status == 2){
        // * Falling
        if(this->bCollision(adjustX, adjustY + abs(jumpStep))){
            g_playerRect.y += m_pPlayerCollision.y - PLAYER_HEIGHT - (g_playerRect.y + velocityY);
            jump_status = 0;
        } else{
            velocityY += abs(jumpStep);
            g_playerRect.y += velocityY;
            jumpStep += G_FALL_SPEED;      
        }

        // * Check touch ground
        bool bTouchGround = g_playerRect.y + PLAYER_HEIGHT + velocityY + jumpStep > Game::GROUND_HEIGHT;
        if(bTouchGround){
            g_playerRect.y += Game::GROUND_HEIGHT - PLAYER_HEIGHT - g_playerRect.y;
            jump_status = 0;
        }
    } else if(jump_status == 0){
        // * Gravity
        if(!this->bCollision(adjustX, adjustY + G_SPEED_GRAVITY)){
            g_playerRect.y += G_SPEED_GRAVITY;
        }
    }
}

void Player::on_short()
{
    int posX, posY, speedX, speedY;
    
    if(g_lastDirection == DIRECTION.RIGHT) {
        posX = g_playerRect.x + PLAYER_WIDTH;
        posY = g_playerRect.y;
        speedX = Bullet::G_SPEED_BULLET;
        speedY = 0;
    } else if(g_lastDirection == DIRECTION.LEFT){
        posX = g_playerRect.x;
        posY = g_playerRect.y;
        speedX = -Bullet::G_SPEED_BULLET;
        speedY = 0;
    } else {
        throw std::runtime_error("ERROR empty stack");
    }
    
    // * Adjust bullet position
    posY += PLAYER_HEIGHT / 2;
    
    std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>
    (
     "assets/imgs/d1.png", posX, posY, 20, 20, speedX, speedY
    );
    g_vBullet_bucket.emplace_back(*bullet);
}

bool Player::bCollision(const int playerX, const int playerY)
{
    for(size_t i = 0; i < Game::g_vObstacle_bucket.size(); ++i){
        float objectX = Game::g_vObstacle_bucket[i].obstacleRect.x;
        float objectY = Game::g_vObstacle_bucket[i].obstacleRect.y;
        float objectWidth = Game::g_vObstacle_bucket[i].obstacleRect.w;
        float objectHeight = Game::g_vObstacle_bucket[i].obstacleRect.h;
        
        if(
           (playerX >= objectX - PLAYER_WIDTH + 1 &&
            playerX <= objectX + objectWidth - 1 &&
            playerY >= objectY - PLAYER_HEIGHT + 1 &&
            playerY <= objectY + objectHeight - 1) ||
           (playerY + PLAYER_HEIGHT > Game::GROUND_HEIGHT)
           ){
            m_pPlayerCollision = Game::g_vObstacle_bucket[i].obstacleRect;
            return true;
        }
    }
    return false;
}

void Player::update_bullet()
{
    for(int i = 0; i < g_vBullet_bucket.size(); ++i){
        g_vBullet_bucket[i].update(i);
    }
}

void Player::draw_bullet()
{
    for(size_t i = 0; i < g_vBullet_bucket.size(); ++i){
        Texture::DrawTexture(Bullet::b_pBulletTexture, NULL, &g_vBullet_bucket[i].bulletRect);
    }
}



