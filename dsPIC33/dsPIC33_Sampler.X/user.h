/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

/* TODO Application specific user parameters used in user.c may go here */

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

typedef enum ChanConf {USER_ONE_CHAN, USER_FOUR_CHAN} USER_ADC_CONF;

void InitApp( USER_ADC_CONF config ); /* I/O and Peripheral Initialization */
void setLed(bool val);          /* Simple LED control. */
