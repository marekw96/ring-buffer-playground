#pragma once

#include <atomic>
#include <print>

template <typename T, unsigned Size>
class atomic_ring_buffer {
    struct entry{
        struct {
            unsigned seq : 31;
            unsigned used : 1  = 0;
        };
        T value{};
    };
public:
    atomic_ring_buffer() {
        write_pos.store(0);
        read_pos.store(0);

        for(auto i = 0u; i < Size; ++i){
            entry e{};
            e.seq = i;
            e.used = 0;
            elements[i].store(e);
        }
    }

    bool put(const T& element_to_put) {
        while(true){
            unsigned wr_index = write_pos.load();
            auto wr_entry = elements[wr_index % Size].load();
            auto seq = wr_entry.seq;

            if(seq == wr_index){
                entry data_entry;
                data_entry.seq = wr_index;
                data_entry.used = 1;
                data_entry.value = element_to_put;

                if(elements[wr_index % Size].compare_exchange_strong(wr_entry, data_entry)){
                    write_pos.compare_exchange_strong(wr_index, wr_index + 1);
                    return true;
                }
            }
            else if((seq + Size) == wr_index){
                //queue is full
                return false;
            }
        }
        return false;
    }

    std::optional<T> get() {
        unsigned rd_index = read_pos.load();
        while(true){
            entry e = elements[rd_index % Size].load();

            if(e.seq == rd_index && e.used == 1){
                entry empty_entry;
                empty_entry.seq = rd_index + Size;

                if(elements[rd_index % Size].compare_exchange_strong(e, empty_entry)){
                    read_pos.compare_exchange_strong(rd_index, rd_index + 1);
                    return e.value;
                }
            }
            else if(e.seq == rd_index){
                //queue is empty
                return std::nullopt;
            }

            rd_index = read_pos.load();
        }
    }

private:
    std::atomic<unsigned> write_pos;
    std::atomic<unsigned> read_pos;
    std::atomic<entry> elements[Size]{};
};