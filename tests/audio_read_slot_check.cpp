#include "dsp/AudioReadSlot.h"
#include <thread>
#include <iostream>
#include <cstdlib>

int main()
{
    v4jb::AudioReadSlot<int> slot;
    auto old=std::make_shared<int>(12345);std::weak_ptr<int> weak=old;
    slot.publish(old);old.reset();
    std::atomic<int> stage{0};
    std::thread reader([&]{auto* value=slot.acquire();stage.store(1);while(stage.load()!=2)std::this_thread::yield();if(!value || *value!=12345)std::abort();slot.release();});
    while(stage.load()!=1)std::this_thread::yield();
    for(int i=0;i<10000;++i)slot.publish(std::make_shared<int>(i));
    if(weak.expired())std::abort();stage.store(2);reader.join();slot.collect();if(!weak.expired())return 1;
    // Real concurrent publish/read/retire, including acquire validation races.
    std::atomic<bool> done{false};std::atomic<unsigned> reads{0};
    slot.acquire(); // establish the last-known valid wave before the writer races
    std::thread audio([&]{slot.acquire();do{const auto* p=slot.acquire();if(!p)std::abort();if(*p<0||*p>=10000)std::abort();++reads;}while(!done.load());slot.release();});
    for(int i=0;i<10000;++i)slot.publish(std::make_shared<int>(i));
    done.store(true);audio.join();slot.collect();
    if(reads.load()==0)return 1;
    std::cout<<"audio_read_slot_check PASS, "<<reads.load()<<" protected reads\n";
}
