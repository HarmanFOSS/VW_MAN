#ifndef PERSISTENCE_CLIENT_LIBRARY_RC_TABLE_H
#define PERSISTENCE_CLIENT_LIBRARY_RC_TABLE_H

/******************************************************************************
 * Project         Persistency
 * (c) copyright   2012
 * Company         XS Embedded GmbH
 *****************************************************************************/
/******************************************************************************
 * This Source Code Form is subject to the terms of the
 * Mozilla Public License, v. 2.0. If a  copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
******************************************************************************/
 /**
 * @file           persistence_client_library_rc_table.h
 * @ingroup        Persistence client library
 * @author         Ingo Huerner
 * @brief          Header of the persistence client library resource config table.
 * @see            
 */

#ifdef __cplusplus
extern "C" {
#endif

#define  PERSIST_DATA_RC_TABLE_INTERFACE_VERSION   (0x01000000U)

#include "persistence_client_library_data_organization.h"


/// enumerator used to identify the policy to manage the data
typedef enum _PersistencePolicy_e
{
   PersistencePolicy_wc    = 0,  /**< the data is managed write cached */
   PersistencePolicy_wt    = 1,  /**< the data is managed write through */
   PersistencePolicy_na    = 2,  /**< the data is not applicable */

   /** insert new entries here ... */
   PersistencePolicy_LastEntry         /**< last entry */

} PersistencePolicy_e;


/// enumerator used to identify the persistence storage to manage the data
typedef enum _PersistenceStorage_e
{
   PersistenceStorage_local    = 0,  /**< the data is managed local */
   PersistenceStorage_shared   = 1,  /**< the data is managed shared */
   PersistenceStorage_custom   = 2,  /**< the data is managed over custom client implementation */

   /** insert new entries here ... */
   PersistenceStoragePolicy_LastEntry         /**< last entry */

} PersistenceStorage_e;


/// structure used to manage database context
typedef struct _PersistenceDbContext_s
{
   unsigned int ldbid;
   unsigned int user_no;
   unsigned int seat_no;
} PersistenceDbContext_s;


/// structure used to manage the persistence configuration for a key
typedef struct _PersistenceConfigurationKey_s
{
   PersistencePolicy_e  policy;                                /**< policy  */
   PersistenceStorage_e storage;                               /**< definition of storage to use */
   unsigned int         permission;                            /**< access right, corresponds to UNIX */
   unsigned int         max_size;                              /**< max size expected for the key */
   char                 reponsible[MaxConfKeyLengthResp];      /**< name of responsible application */
   char                 custom_name[MaxConfKeyLengthCusName];  /**< name of the customer plugin */
} PersistenceConfigurationKey_s;


/// structure definition of an persistence resource configuration table entry
typedef struct _PersistenceRctEntry_s
{
    char key[PrctKeySize];                                   /**< the key */
    PersistenceConfigurationKey_s data;                      /**< data for the key */
}
PersistenceRctEntry_s;


/// persistence information
typedef struct _PersistenceInfo_s
{
   PersistenceDbContext_s           context;          /**< database context*/
   PersistenceConfigurationKey_s    configKey;        /**< prct configuration key*/

} PersistenceInfo_s;



/// persistence resource config table type definition
typedef enum _PersistenceRCT_e
{
   PersistenceRCT_local         = 0,
   PersistenceRCT_shared_public = 1,
   PersistenceRCT_shared_group  = 2,

   PersistenceRCT_LastEntry                // last Entry

} PersistenceRCT_e;



#ifdef __cplusplus
}
#endif

#endif /* PERSISTENCY_CLIENT_LIBRARY_RC_TABLE_H */
