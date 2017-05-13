#pragma once

#include <ingameelements/projectile.h>


class Helixrocket : public Projectile
{
    public:
        virtual void init(uint64_t id_, Gamestate &state, EntityPtr owner_) override;
        virtual ~Helixrocket() override = default;
        void beginstep(Gamestate &state, double frametime) override;
        void endstep(Gamestate &state, double frametime) override {}
        std::unique_ptr<Entity> clone() override {return std::unique_ptr<Entity>(new Helixrocket(*this));}
        void render(Renderer &renderer, Gamestate &state) override;
        double explode(Gamestate &state);
        // Flashbang deals all its damage in explode() which is called in destroy() which registers ult charge itself
        double dealdamage(Gamestate &state, Entity &target) override {return 0;}
        void destroy(Gamestate &state) override;
        PenetrationLevel penetration() override {return PENETRATE_NOTHING;}
        std::string spriteid() override {return "heroes/mccree/projectiles/helixrocket";}

        Timer countdown;
    protected:
    private:
        double EXPLOSION_RADIUS = 40;
};

