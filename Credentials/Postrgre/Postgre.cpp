#include <iostream>
#include "Postgre.h"
#include <acess.h>

Postgre::Postgre()
{
    
}

pqxx::connection* Postgre::Connect()
{
    
    
    try {
        Acess cred;
        std::string connectionStr = "user=" + cred.getDBusername() +
            " password=" + cred.getDBpwd() +
            " hostaddr=" + cred.getDBhost() +
            " port=" + cred.getDBport() +
            " dbname =" + cred.getDBname();

        pqxx::connection* conn= new pqxx::connection(connectionStr);
        
        
        if (conn->is_open()) {
            return conn;
        }
        else {
            std::cerr << "Failed to connect to the PostgreSQL server!" << std::endl;
            exit;
        }
        
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        
    }

    
}

void Postgre::TablesCheck()
{
    pqxx::work txn(*Connect());     

    const std::string createGames = R"(
            CREATE TABLE IF NOT EXISTS public.games
            (
                pk_game_appid integer NOT NULL,
                game_name text NOT NULL,
                game_type text,
                game_is_free boolean,
                game_short_description text,
                game_long_description text,
                game_website text,
                game_initial_price numeric,
                game_platform_windows boolean,
                game_platform_mac boolean,
                game_platform_linux boolean,
                game_metacritic_score integer,
                game_genre text,
                game_total_recommendations integer,
                game_coming_soon boolean,
                game_release_date date,
                game_support_url text,
                game_support_mail text,
                game_last_db_update timestamp without time zone,
                PRIMARY KEY (pk_game_appid)
            )
            WITH (
                OIDS = FALSE
            );

            ALTER TABLE IF EXISTS public.games
                OWNER to postgres;
        )";


    const std::string createDevelopers = R"(
            CREATE TABLE IF NOT EXISTS public.developers
            (
                pk_developer_id serial NOT NULL,
                developer text NOT NULL,
                PRIMARY KEY (pk_developer_id)
            )
            WITH (
                OIDS = FALSE
            );

            ALTER TABLE IF EXISTS public.developers
                OWNER to postgres;
        )";

    const std::string createPublishers = R"(
            CREATE TABLE IF NOT EXISTS public.publishers
            (
                pk_publisher_id serial NOT NULL,
                publisher text NOT NULL,
                PRIMARY KEY (pk_publisher_id)
            )
            WITH (
                OIDS = FALSE
            );

            ALTER TABLE IF EXISTS public.publishers
                OWNER to postgres;
        )";

    const std::string createGamePublisher = R"(
            CREATE TABLE IF NOT EXISTS public.game_publisher
            (
                fk_game_appid integer NOT NULL,
                fk_publisher integer NOT NULL,
                CONSTRAINT fk_game_appid FOREIGN KEY (fk_game_appid)
                    REFERENCES public.games (pk_game_appid) MATCH SIMPLE
                    ON UPDATE NO ACTION
                    ON DELETE NO ACTION
                    NOT VALID,
                CONSTRAINT fk_publisher FOREIGN KEY (fk_publisher)
                    REFERENCES public.publishers (pk_publisher_id) MATCH SIMPLE
                    ON UPDATE NO ACTION
                    ON DELETE NO ACTION
                    NOT VALID
            )
            WITH (
                OIDS = FALSE
            );

            ALTER TABLE IF EXISTS public.game_publisher
                OWNER to postgres;
        )";

    const std::string createGameDeveloper = R"(
            CREATE TABLE IF NOT EXISTS public.game_developer
            (
                fk_game_appid integer NOT NULL,
                fk_developer integer NOT NULL,
                CONSTRAINT fk_game_appid FOREIGN KEY (fk_game_appid)
                    REFERENCES public.games (pk_game_appid) MATCH SIMPLE
                    ON UPDATE NO ACTION
                    ON DELETE NO ACTION
                    NOT VALID,
                CONSTRAINT fk_developer FOREIGN KEY (fk_developer)
                    REFERENCES public.developers (pk_developer_id) MATCH SIMPLE
                    ON UPDATE NO ACTION
                    ON DELETE NO ACTION
                    NOT VALID
            )
            WITH (
                OIDS = FALSE
            );

            ALTER TABLE IF EXISTS public.game_developer
                OWNER to postgres;
        )";

    const std::string createDLC = R"(
            CREATE TABLE IF NOT EXISTS public.game_dlc
            (
                fk_game_appid integer NOT NULL,
                dlc integer NOT NULL,
                CONSTRAINT fk_game_appid FOREIGN KEY (fk_game_appid)
                    REFERENCES public.games (pk_game_appid) MATCH SIMPLE
                    ON UPDATE NO ACTION
                    ON DELETE NO ACTION
                    NOT VALID   
            )
            WITH (
                OIDS = FALSE
            );

            ALTER TABLE IF EXISTS public.game_dlc
                OWNER to postgres;
        )";

    const std::string createGenres = R"(
            CREATE TABLE IF NOT EXISTS public.genres
            (
                pk_genre_id integer NOT NULL,
                genre_name text NOT NULL,
                PRIMARY KEY (pk_genre_id)
            )
            WITH (
                OIDS = FALSE
            );

            ALTER TABLE IF EXISTS public.genres
                OWNER to postgres;
        )";

    const std::string createCategories = R"(
            CREATE TABLE IF NOT EXISTS public.categories
            (
                pk_category_id integer NOT NULL,
                category_name text NOT NULL,
                PRIMARY KEY (pk_category_id)
            )
            WITH (
                OIDS = FALSE
            );

            ALTER TABLE IF EXISTS public.categories
                OWNER to postgres;
        )";
    const std::string createGameGenres = R"(
            CREATE TABLE IF NOT EXISTS public.game_genres
            (
                fk_game_appid integer NOT NULL,
                fk_genre_id integer NOT NULL,
                CONSTRAINT fk_game_appid FOREIGN KEY (fk_game_appid)
                    REFERENCES public.games (pk_game_appid) MATCH SIMPLE
                    ON UPDATE NO ACTION
                    ON DELETE NO ACTION
                    NOT VALID,
                CONSTRAINT fk_genre_id FOREIGN KEY (fk_genre_id)
                    REFERENCES public.genres (pk_genre_id) MATCH SIMPLE
                    ON UPDATE NO ACTION
                    ON DELETE NO ACTION
                    NOT VALID
            )
            WITH (
                OIDS = FALSE
            );

            ALTER TABLE IF EXISTS public.game_genres
                OWNER to postgres;
        )";
    const std::string createGameCategory = R"(
            CREATE TABLE IF NOT EXISTS public.game_category
            (
                fk_game_appid integer NOT NULL,
                fk_category_id integer NOT NULL,
                CONSTRAINT fk_game_appid FOREIGN KEY (fk_game_appid)
                    REFERENCES public.games (pk_game_appid) MATCH SIMPLE
                    ON UPDATE NO ACTION
                    ON DELETE NO ACTION
                    NOT VALID,
                CONSTRAINT fk_category_id FOREIGN KEY (fk_category_id)
                    REFERENCES public.categories (pk_category_id) MATCH SIMPLE
                    ON UPDATE NO ACTION
                    ON DELETE NO ACTION
                    NOT VALID
            )
            WITH (
                OIDS = FALSE
            );

            ALTER TABLE IF EXISTS public.game_category
                OWNER to postgres;
        )";

    const std::string createGameLanguage = R"(
            CREATE TABLE IF NOT EXISTS public.game_language
            (
                fk_game_appid integer NOT NULL,
                language text NOT NULL,
                CONSTRAINT fk_game_appid FOREIGN KEY (fk_game_appid)
                    REFERENCES public.games (pk_game_appid) MATCH SIMPLE
                    ON UPDATE NO ACTION
                    ON DELETE NO ACTION
                    NOT VALID
            )
            WITH (
                OIDS = FALSE
            );

            ALTER TABLE IF EXISTS public.game_language
                OWNER to postgres;
        )";



    txn.exec(createGames);    
    txn.exec(createPublishers);    
    txn.exec(createDevelopers);   
    txn.exec(createGamePublisher);   
    txn.exec(createGameDeveloper); 
    txn.exec(createDLC);
    txn.exec(createGenres);
    txn.exec(createCategories);
    txn.exec(createGameGenres);
    txn.exec(createGameCategory);
    txn.exec(createGameLanguage);

    txn.commit();

}

Postgre::~Postgre()
{
}

