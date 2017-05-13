#pragma once

#include "ingameelements/weapon.h"
#include "ingameelements/clipweapon.h"
#include <unordered_map>

class Pulserifle : public Clipweapon
{
    public:
        virtual void init(uint64_t id_, Gamestate &state, EntityPtr owner_) override;
        virtual ~Pulserifle() override = default;

        void render(Renderer &renderer, Gamestate &state) override;
        std::unique_ptr<Entity> clone() override {return std::unique_ptr<Entity>(new Pulserifle(*this));}

        void fireprimary(Gamestate &state) override;
        void wantfireprimary(Gamestate &state) override;
        void firesecondary(Gamestate &state) override;
        void wantfiresecondary(Gamestate &state) override;
        void fireultimate(Gamestate &state);
        void beginstep(Gamestate &state, double frametime) override;
        void reload(Gamestate &state) override;

        std::string herofolder() override {return "heroes/mccree/";}
        int getclipsize() override {return 6;}
        std::function<void(Gamestate &state)> getreloadfunction(Gamestate &state) override {return std::bind(&Pulserifle::restoreclip, this, std::placeholders::_1);}
        double getattachpoint_x(Gamestate &state) override {return -3;}
        double getattachpoint_y(Gamestate &state) override {return 8;}

        Animation fthanim;
        bool isfthing;

        std::unordered_map<int, double> deadeyetargets;
        Animation deadeyeanim;
        bool isfiringult;
    protected:
    private:
        double MAX_DAMAGE = 70;
        double MAX_FTH_DAMAGE = 45;
        double FALLOFF_BEGIN = 30*10;
        double FALLOFF_END = 30*20;
};

