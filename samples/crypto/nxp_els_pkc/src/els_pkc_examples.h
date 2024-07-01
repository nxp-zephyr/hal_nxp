/**
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _ELS_PKC_EXAMPLES_H_
#define _ELS_PKC_EXAMPLES_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXIT_CODE_ERROR 0x00U
#define EXIT_CODE_OK    0x01U
/*******************************************************************************
 * API
 ******************************************************************************/

extern bool mcuxClHashModes_sha224_oneshot_example(void);
extern bool mcuxClHashModes_sha256_oneshot_example(void);
extern bool mcuxClHashModes_sha256_longMsgOneshot_example(void);
extern bool mcuxClHashModes_sha256_streaming_example(void);
extern bool mcuxClHashModes_sha384_oneshot_example(void);
extern bool mcuxClHashModes_sha512_oneshot_example(void);

extern bool mcuxClEls_Ecc_Keygen_Sign_Verify_example(void);
extern bool mcuxClRsa_sign_NoEncode_example(void);
extern bool mcuxClRsa_sign_pss_sha2_256_example(void);
extern bool mcuxClRsa_verify_NoVerify_example(void);
extern bool mcuxClRsa_verify_pssverify_sha2_256_example(void);
extern bool mcuxClEcc_MontDH_Curve25519_example(void);
extern bool mcuxClEls_Tls_Master_Key_Session_Keys_example(void);

extern bool mcuxClEls_Rng_Prng_Get_Random_example(void);
extern bool mcuxClEls_Common_Get_Info_example(void);
extern bool mcuxCsslFlowProtection_example(void);
extern uint32_t mcuxCsslMemory_Compare_example(void);
extern uint32_t mcuxCsslMemory_Copy_example(void);
extern bool mcuxClKey_example(void);

extern bool mcuxClEls_Cipher_Aes128_Cbc_Encrypt_example(void);
extern bool mcuxClEls_Cipher_Aes128_Ecb_Encrypt_example(void);
extern bool mcuxClMacModes_Els_Cmac_Aes128_Oneshot_example(void);

#endif /* _ELS_PKC_EXAMPLES_H_ */
