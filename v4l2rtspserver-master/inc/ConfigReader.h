 #ifndef DEF_ConfigReader
 #define DEF_ConfigReader

 class ConfigReader
 {
 public:
    static ConfigReader& instance()
    {
       static ConfigReader _instance;
       return _instance;
    }
    ~ConfigReader() {}
    void readConfig();
 private:
    ConfigReader() {}           // verhindert, dass ein Objekt von außerhalb von ConfigReader erzeugt wird.
                    // protected, wenn man von der Klasse noch erben möchte
    ConfigReader( const ConfigReader& ); /* verhindert, dass eine weitere Instanz via
 Kopier-Konstruktor erstellt werden kann */
    ConfigReader & operator = (const ConfigReader &); //Verhindert weitere Instanz durch Kopie
 };


#endif