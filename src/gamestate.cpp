#include <unordered_map>
#include <memory>

#include "gamestate.h"
#include "entity.h"
#include "ingameelements/player.h"

Gamestate::Gamestate(Engine *engine_) : entitylist(), playerlist(), currentmap(), engine(engine_), entityidcounter(1)
{
    time = 0;
}

Gamestate::~Gamestate()
{
    ;
}

void Gamestate::update(double frametime)
{
    time += frametime;

    for (auto& e : entitylist)
    {
        if (e.second->isrootobject() and not e.second->destroyentity)
        {
            e.second->beginstep(this, frametime);
        }
    }
    for (auto& e : entitylist)
    {
        if (e.second->isrootobject() and not e.second->destroyentity)
        {
            e.second->midstep(this, frametime);
        }
    }
    for (auto& e : entitylist)
    {
        if (e.second->isrootobject() and not e.second->destroyentity)
        {
            e.second->endstep(this, frametime);
        }
    }
    for (auto e = entitylist.begin(); e != entitylist.end();)
    {
        if (e->second->destroyentity)
        {
            e = entitylist.erase(e);
        }
        else
        {
            ++e;
        }
    }
}

EntityPtr Gamestate::addplayer()
{
    EntityPtr r = make_entity<Player>(this);
    playerlist.push_back(r);
    return r;
}

void Gamestate::removeplayer(int playerid)
{
    get<Player>(playerlist[playerid])->destroy(this);
    playerlist.erase(playerlist.begin()+playerid);
}

std::unique_ptr<Gamestate> Gamestate::clone()
{
    std::unique_ptr<Gamestate> g = std::unique_ptr<Gamestate>(new Gamestate(engine));
    for (auto& e : entitylist)
    {
        g->entitylist[e.second->id] = e.second->clone();
    }
    g->time = time;
    g->entityidcounter = entityidcounter;
    g->currentmap = currentmap;
    return g;
}

void Gamestate::interpolate(Gamestate *prevstate, Gamestate *nextstate, double alpha)
{
    // Use threshold of alpha=0.5 to decide binary choices like entity existence
    Gamestate *preferredstate;
    if (alpha < 0.5)
    {
        preferredstate = prevstate;
    }
    else
    {
        preferredstate = nextstate;
    }
    currentmap = preferredstate->currentmap;

    entitylist.clear();
    for (auto& e : preferredstate->entitylist)
    {
        entitylist[e.second->id] = e.second->clone();
        if (prevstate->entitylist.count(e.second->id) != 0 && nextstate->entitylist.count(e.second->id) != 0)
        {
            // If the instance exists in both states, we need to actually interpolate values
            entitylist[e.second->id]->interpolate(prevstate->entitylist[e.second->id].get(), nextstate->entitylist[e.second->id].get(), alpha);
        }
    }
}

void Gamestate::serializesnapshot(WriteBuffer *buffer)
{
    for (auto p : playerlist)
    {
        get<Player>(p)->serialize(this, buffer, false);
    }
}

void Gamestate::deserializesnapshot(ReadBuffer *buffer)
{
    for (auto p : playerlist)
    {
        get<Player>(p)->deserialize(this, buffer, false);
    }
}

void Gamestate::serializefull(WriteBuffer *buffer)
{
    buffer->write<uint32_t>(playerlist.size());
    for (auto p : playerlist)
    {
        get<Player>(p)->serialize(this, buffer, true);
    }
}

void Gamestate::deserializefull(ReadBuffer *buffer)
{
    int nplayers = buffer->read<uint32_t>();
    for (int i=0; i<nplayers; ++i)
    {
        addplayer();
    }
    for (auto p : playerlist)
    {
        get<Player>(p)->deserialize(this, buffer, true);
    }
}
