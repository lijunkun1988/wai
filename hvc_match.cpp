#include <time.h>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "common_define.h"
#include "hvc_match.h"
#include "sandbox.h"

const int BUF_LEN = 1024;

void HVCMatch::Start() {
    m_start_time = time(NULL);
    m_judge->Run();
    m_computer->Run();
    char buf[BUF_LEN], tmp_buf[BUF_LEN];
    while (1) {
        memset(buf, 0, sizeof(buf));
        m_judge->Recv(buf, BUF_LEN-1);
        fprintf(stderr, "The judge said {%s}", buf);
        if (buf[0] == '>') {
            int dst = buf[1] - '1';
            if (dst == 0) {
                // to human: display the computer's move
                printf("%s\n", buf+4);
                fflush(stdout); // neccessary?
            } else if (dst == 1) {
                // computer
                m_computer->Send(buf+4);
            } else assert(false);
        } else if (buf[0] == '<') {
            int src = buf[1] - '1';
            if (src == 0) {
                // from human: request move from browser
                // scanf("%s", tmp_buf);
                fgets(tmp_buf, BUF_LEN-1, stdin); 
                m_judge->Send(tmp_buf);
            } else if (src == 1) {
                if (m_computer->Recv(buf, BUF_LEN-1) == 0) {
                    fprintf(stderr, "Computer exited, type: %d\n",
                            m_computer->GetExitType());
                    break;
                }
                m_judge->Send(buf);
            } else assert(false);
        } else if (isdigit(buf[0])) {
            char res_str[BUF_LEN], reason[BUF_LEN];
            sscanf(buf, "%d %s %s", &m_winner, res_str, reason);
            printf(": %d %s %s\n", m_winner, res_str, reason);
            fflush(stdout);
            break;
        } // ignore '+'
    }
    m_end_time = time(NULL);
};

