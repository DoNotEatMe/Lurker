//internal
#include <Postgre.h>
#include <acess.h>
//system
#include <iostream>
//vcpkg
#include <pqxx/pqxx>

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

        pqxx::connection* Connection = new pqxx::connection(connectionStr);
        
        
        if (Connection->is_open()) {
            return Connection;
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
                developer_name text UNIQUE NOT NULL,
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
                publisher_name text UNIQUE NOT NULL,
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
                fk_publisher_id integer NOT NULL,
                CONSTRAINT fk_game_appid FOREIGN KEY (fk_game_appid)
                    REFERENCES public.games (pk_game_appid) MATCH SIMPLE
                    ON UPDATE NO ACTION
                    ON DELETE NO ACTION
                    NOT VALID,
                CONSTRAINT fk_publisher FOREIGN KEY (fk_publisher_id)
                    REFERENCES public.publishers (pk_publisher_id) MATCH SIMPLE
                    ON UPDATE NO ACTION
                    ON DELETE NO ACTION
                    NOT VALID,
                CONSTRAINT unique_game_publisher UNIQUE (fk_game_appid, fk_publisher_id)
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
                fk_developer_id integer NOT NULL,
                CONSTRAINT fk_game_appid FOREIGN KEY (fk_game_appid)
                    REFERENCES public.games (pk_game_appid) MATCH SIMPLE
                    ON UPDATE NO ACTION
                    ON DELETE NO ACTION
                    NOT VALID,
                CONSTRAINT fk_developer FOREIGN KEY (fk_developer_id)
                    REFERENCES public.developers (pk_developer_id) MATCH SIMPLE
                    ON UPDATE NO ACTION
                    ON DELETE NO ACTION
                    NOT VALID,
                CONSTRAINT unique_game_developer UNIQUE (fk_game_appid, fk_developer_id)
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
                dlc_id integer NOT NULL,
                dlc_type text,
                CONSTRAINT fk_game_appid FOREIGN KEY (fk_game_appid)
                    REFERENCES public.games (pk_game_appid) MATCH SIMPLE
                    ON UPDATE NO ACTION
                    ON DELETE NO ACTION
                    NOT VALID,
                CONSTRAINT unique_game_dlc UNIQUE (fk_game_appid, dlc_id)
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
                PRIMARY KEY (pk_genre_id),
                CONSTRAINT unique_genre_id_name UNIQUE (pk_genre_id, genre_name)
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
                PRIMARY KEY (pk_category_id),
                CONSTRAINT unique_category_id_name UNIQUE (pk_category_id, category_name)
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
                    NOT VALID,
                CONSTRAINT unique_game_genres UNIQUE (fk_game_appid, fk_genre_id)
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
                    NOT VALID,
                CONSTRAINT unique_game_category UNIQUE (fk_game_appid, fk_category_id)
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

    const std::string createLogger = R"(
            CREATE TABLE public.logger
            (
                pk_log_id serial NOT NULL,
                programm_name text NOT NULL,
                game_appid int,
                message text NOT NULL,
                type text NOT NULL,
                "time" timestamp without time zone NOT NULL,
                PRIMARY KEY(pk_log_id)
            )
                WITH(
                    OIDS = FALSE
                );

            ALTER TABLE IF EXISTS public.logger
                OWNER to postgres;
            )";

    const std::string createStatus = R"(
            CREATE TABLE public.status
            (
                programm_id serial UNIQUE NOT NULL,
                programm_name text UNIQUE NOT NULL,
                is_work boolean NOT NULL,
                "time" timestamp without time zone NOT NULL
            )
                WITH(
                    OIDS = FALSE
                );

            ALTER TABLE IF EXISTS public.status
                OWNER to postgres;
            )";


    txn.exec("SET application_name = 'Postgres core'");
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
    txn.exec(createLogger);
    txn.exec(createStatus);

    txn.commit();

}

Postgre::~Postgre()
{
}

