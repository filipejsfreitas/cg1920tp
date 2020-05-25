#ifndef CG_TP_ENGINE_SRC_ENTITIES_ENTITY_H_
#define CG_TP_ENGINE_SRC_ENTITIES_ENTITY_H_

#include <tinyxml2/tinyxml2.h>
#include <iostream>

#include "../Scene.h"

namespace engine::scene::entities {
    class Entity {
    private:
        const Scene& scene;

    protected:
        const Scene& GetScene() const {
            return scene;
        }

    public:
        Entity() = delete;
        Entity(const Scene& scene): scene(scene) {}
        virtual ~Entity() = default;

        Entity(const Entity& entity) = delete;
        Entity& operator=(const Entity& other) = delete;

        /**
         * Função responsável pela leitura da secção do ficheiro XML corresponde a esta entidade.
         * @param node O nodo do ficheiro XML correspondente a esta entidade.
         * @return true se o parsing se deu corretamente; false caso contrário
         */
        virtual bool ParseXml(const tinyxml2::XMLNode *node) = 0;

        /**
         * Função responsável por atualizar os parãmetros internos de cada identidade, se necessário.
         * @param deltaTime
         */
        virtual void Update(double deltaTime) {};

        /**
         * Função responsável por renderizar esta entidade no ecrã.
         */
        virtual void Render() const = 0;
    };
}

#endif //CG_TP_ENGINE_SRC_ENTITIES_ENTITY_H_
