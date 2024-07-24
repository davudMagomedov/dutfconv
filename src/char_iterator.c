#include "char_iterator.h"

char char_iterator_next(struct char_iterator_s* self) {
    if (self->howfar--) {
        return *(self->finger++);
    }
}
