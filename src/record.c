#include "record.h"
#include "config.h"

#include <stdio.h>

static unsigned long RECORD = 0;

void save_record(unsigned long score)
{
    if (score <= get_record()) {
        return;
    }

    FILE *f = fopen(RECORD_FILE_PATH, "wb");
    if (f) {
        fwrite(&score, sizeof(unsigned long), 1, f);
        RECORD = score;
        fclose(f);
    }
}

unsigned long get_record()
{
    if (RECORD) {
        return RECORD;
    }

    FILE *f = fopen(RECORD_FILE_PATH, "rb");
    if (f) {
        fread(&RECORD, sizeof(unsigned long), 1, f);
        fclose(f);
    }
    return RECORD;
}

