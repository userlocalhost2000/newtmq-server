#include <newt/common.h>
#include <newt/stomp_management_worker.h>

#include <assert.h>

static int handler_id(char *context, void *data) {
  stomp_msginfo_t *msginfo = (stomp_msginfo_t *)data;
  int ret = RET_ERROR;

  if(msginfo != NULL) {
    memcpy(msginfo->id, context, strlen(context));
    SET(msginfo, ID_IS_SET);

    ret = RET_SUCCESS;
  }

  return ret;
}

static stomp_header_handler_t handlers[] = {
  {"id:", handler_id},
  {0},
};

frame_t *handler_stomp_nack(frame_t *frame) {
  stomp_msginfo_t *msginfo;

  assert(frame != NULL);
  assert(frame->cinfo != NULL);

  msginfo = alloc_msginfo();
  if(msginfo == NULL) {
    return NULL;
  }

  if(iterate_header(&frame->h_attrs, handlers, msginfo) == RET_ERROR) {
    err("(handle_stomp_nack) validation error");
    stomp_send_error(frame->sock, "failed to validate header\n");
    return NULL;
  }

  /* implementation for nack */

  free_msginfo(msginfo);

  return NULL;
}
