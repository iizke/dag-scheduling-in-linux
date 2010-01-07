/**
 * phase_def.h
 *
 *  Created on: Dec 9, 2009
 *      Author: rosemanit
 */

#ifndef PHASE_DEF_H_
#define PHASE_DEF_H_

#define SUCCESS                                 1
#define FAIL                                    0

#define ERR_PHASE_DAG_NULL                      (-1)
#define ERR_TASK_POOL_NULL                      (-2)
#define ERR_PHASE_TASK_NULL                     (-3)
#define ERR_PHASE_LINK_POOL_NULL                (-4)
#define ERR_PHASE_TASK_LIST_NULL                (-5)
#define ERR_PHASE_LINK_NULL                     (-6)
#define ERR_LINK_POOL_NULL                      (-7)
#define ERR_TASK_POOL_FULL                      (-8)
#define ERR_TASK_POOL_FIND_TASK_FAILED          (-9)

#define TASK_POOL_STATE_UNUSED                  0
#define TASK_POOL_STATE_INUSED                  1

#define LINK_POOL_STATE_UNUSED                  0
#define LINK_POOL_STATE_INUSED                  1

#endif /* PHASE_DEF_H_ */
