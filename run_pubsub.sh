#!/bin/bash
test="small"

if [[ "$test" == "small" ]]; then
    # small payload
    for m_max in 5 10 #2 5 10
    do
        for hz in 100
        do
            ((wait_time=505/hz+2))
            for n_max in 5
            do
                for data in 8B 80B 200B 500B 1000B 2000B
                do
                    ## open n*subscriber and m*publisher
                    for m in $(seq 0 $((m_max-1)))
                    do
                        ./pubtime $data $hz $m & 
                        for n in $(seq 0 $((n_max-1)))
                        do
                            ./subtime $data $m $((n+m*n_max)) & 
                        done
                    done
                    ./sys_stats $data $hz $n_max $m_max &
                    wait
                    python3 data_process.py $hz $n_max $m_max $test
                    sleep 0.5
                done
                python3 cpu_mem_process.py $hz $n_max $m_max $test
                sleep 0.5
            done
        done
    done
else
    for m_max in 1 #2 5 10
    do
        for hz in 10
        do
            ((wait_time=505/hz+5))
            for n_max in 1 5 10
            do
                for data in 1MB 5MB 10MB 20MB 30MB 50MB
                do
                    ## open n*subscriber and m*publisher
                    for m in $(seq 0 $((m_max-1)))
                    do
                        timeout $wait_time ./pubtime $data $hz $m & 
                        for n in $(seq 0 $((n_max-1)))
                        do
                            timeout $wait_time ./subtime $data $m $n & 
                        done
                    done
                    timeout $wait_time ./sys_stats $data $hz $n_max $m_max &
                    wait
                    python3 data_process.py $hz $n_max $m_max $test
                    sleep 0.5
                done
                python3 cpu_mem_process.py $hz $n_max $m_max $test
                sleep 0.5
            done
        done
    done
fi