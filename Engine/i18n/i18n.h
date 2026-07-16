#ifndef I18N_H
#define I18N_H



/**=======================================
🌐 Funções de internacionalização (i18n)  
 
 Permite carregar arquivos JSON com traduções e acessar os textos
 através da função T("chave").
 
 ========================================*/


//  Carrega um arquivo JSON com traduções a partir do caminho especificado.
// `"Projects/RoboCup/Assets/lang/pt.json"`
void I18N_Load_File(const char* filename);


/** @brief Retorna o texto traduzido para a chave especificada.
 *  @param key A chave do texto a ser retornado.
 *  @return O texto traduzido ou a chave se não encontrado.
 */
const char* T(const char* key);


/** @brief Descarrega os recursos de internacionalização.
 *  @return Nada.
 * 
 *  TODO: Implementar a função de descarregamento de recursos.
 */
void I18N_Unload(void);

#endif
